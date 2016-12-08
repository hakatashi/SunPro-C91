= 深層学習で芸術家の脳内を再現する

//raw[|latex|\\chapterauthor{hiromu}]

== はじめに

こんにちは。SunProメンバーのhiromu(@hiromu1996)です。
今回は、2016年11月下旬に発表された「pix2pix」@<bib>{phillip}を使って、
絵から芸術家の脳内を再現するということに挑戦してみました。
一体どういうことなのかというと、アメリカン・コミックのような画風で知られる、
ポップアートの代表的な画家、ロイ・リキテンスタインの絵画から
彼が脳内で思い浮かべていたであろう風景を再現するシステムを作ってみた、という記事となります。

ロイ・リキテンスタインといっても、あんまりイメージが浮かばないかもしれませんが、
代表作の1つである@<img>{hairribbon}の「ヘアリボンの少女」は見たことがある人もいるんじゃないかと思います。
今回は、そうしたリキテンスタインの作品の中でも、
@<img>{interior}のように室内の風景をモデルにした「インテリア」シリーズを対象とします@<fn>{whyinterior}。
//footnote[whyinterior][データセットに関する章まで読み進めて頂くと、その理由が分かるかと思います。]
//image[hairribbon][Roy Lichtenstein「Girl with Hair Ribbon」(1965)@<raw>{|latex|\protect}@<fn>{hairribbon}][scale=0.5]
//footnote[hairribbon][@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=60&year=65&work_id=278}より引用]
//image[interior][Roy Lichtenstein「Large Interior with Three Reflections」(1993)@<raw>{|latex|\protect}@<fn>{interior}][scale=0.9]
//footnote[interior][@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=90&year=93&work_id=1649}より引用]

== pix2pixとは

まずは、pix2pixについて、その概要を説明していきたいと思います。
pix2pixは、画像から画像への変換を必要とする問題に対して包括的に使用できる手法として提案されており、
@<bib>{phillip}では、@<img>{pix2pix}のように白黒写真のカラー化から、航空写真からの地図の生成、あるいは、
線画からの画像の復元にまで適用できることが示されています。
//image[pix2pix][pix2pixの適用例(@<bib>{phillip}より引用)]

このpix2pixは、「敵対的生成ネットワーク(Generative Adverserial Network, GAN)」という技術を基にしています。
GANでは、乱数から画像を生成するGeneratorと、学習データとGeneratorが生成した画像を見分けるDiscriminatorという
2つのニューラルネットワークを競争させながら学習することで、学習データに似たような画像を自動生成することが可能となっています。
例えるなら、Generatorは贋作画家、Discriminatorは鑑定士で、@<img>{gan}のように互いに勝負を繰り返させることで、
贋作画家の画力は少しずつ向上し、鑑定士の能力も高くなっていくという仕組みです。
//image[gan][敵対的生成ネットワーク(GAN)のイメージ図][scale=0.9]

pix2pixは、これを発展させた「条件付き敵対的生成ネットワーク(Conditional GAN)」という技術を使用しています。
Conditional GANでは、航空写真と地図、白黒写真とカラー写真のように入力と出力に対応する2つの画像のペアを学習データとして用います。
そして、Generatorには、航空写真や白黒写真といった入力側となる学習データを与え、その画像に合った画像を生成するようにします。
さらに、Discriminatorには、学習データに含まれる正しいペアと、
入力側だけ学習データで出力側はGeneratorが生成した画像という偽のペアを与え、判別させます。
つまり、@<img>{cgan}のようになり、これによって画像変換をGANの仕組みの中で扱えるようになるのです。
//image[cgan][条件付き敵対的生成ネットワーク(Conditional GAN)のイメージ図(@<bib>{phillip}を基に作成)][scale=0.6]

GANとConditional GANの目的関数は次の通りになります。
比較すると、Generatorに入力側となる学習データ@<m>{x}が与えられるようになり、Discriminatorには
正しいペアの場合に@<m>{D(x, y)}偽のペアの場合に@<m>{D(x, G(x, z))}と2つの画像が与えられるようになっていることが分かるかと思います。
//texequation{
\begin{split}
    \mathcal{L}_{GAN}(G, D) & = \mathbb{E}_{x \sim p_{data}(x))}[\log D(x)] + \mathbb{E}_{z \sim p(z)}[\log (1 - D(G(z))] \\
    \mathcal{L}_{cGAN}(G, D) & = \mathbb{E}_{x, y \sim p_{data}(x, y))}[\log D(x, y)] + \mathbb{E}_{x \sim p_{data}(x), z \sim p(z)}[\log (1 - D(x, G(x,z))]
\end{split}
//}
なお、通常のGANの場合は@<m>{G^* = arg\underset{G\}{min\}\underset{D\}{max\}\ \mathcal{L\}(G, D)}として、最適なGeneratorを得るように学習しますが、
pix2pixでは@<m>{\mathcal{L\}_{L1\}(G) = \mathbb{E\}_{x,y \sim p_{data\}(x, y), z \sim p_z(z)\}[\left \| y - G(x, z) \right \|_1]}というL1正則化の項を追加して、
@<m>{G^* = arg\underset{G\}{min\}\underset{D\}{max\}\ \mathcal{L\}_{cGAN\}(G, D) + \lambda \mathcal{L\}_{L1\}(G)}としています。
これは、生成された画像をピクセル単位で正解に近づけるような制約として作用し、GANでの生成結果がよりリアルになることが知られています@<bib>{deepak}。

さらに、pix2pixではPatchGANと呼ばれる技術を導入しています。
これは、Discriminatorが判定をする際に、画像全体を与えるのではなく@<m>{16 \times 16}や@<m>{70 \times 70}といった小さな領域ごとに区切って与えるという手法で、
画像を大域的に見て判定するというDiscriminatorの機能を失うことなく、パラメータ数を削減することができるため、より効率的に学習を進められるとしています。

そして、pix2pixの実装はすべてGitHub (@<href>{https://github.com/phillipi/pix2pix})に公開されています。
つまり、ペアとなった画像の学習データさえ用意すれば、ここまで説明してきた技術をすぐに使えるというわけです。
こんな素晴らしいものを使わない手はないですよね。

== データセットを作る

とはいっても、ロイ・リキテンスタインの絵画とその元になった写真というようなデータは存在しません。
つまり、pix2pixで絵画を変換するには、まずそのための学習データを作るということから始めないといけません。

ここで、@<img>{interior}を観察してみましょう。
これを見ると、ロイ・リキテンスタインの絵の特徴的なポイントは、以下の3点にあることが分かります。

 * 太く黒い線でしっかりと縁取られた輪郭線
 * 赤や黄といった原色に近い鮮やかな配色
 * 斜線や水玉模様といった幾何学的かつ単純なテクスチャ

逆に考えてみると、写真と、それに写っている物体の輪郭の情報があれば、
機械的に配色し、テクスチャを貼り付けることでロイ・リキテンスタイン風の絵を生成できそうです。

さらに、輪郭抽出はCanny法と呼ばれるアルゴリズムを使えば、OpenCVなどで自動的にできるのでどうにかなるように思えます。
しかし、Canny法で複雑な画像に対して輪郭抽出をしようとすると、かなり細かくパラメータ設定をしないと
@<img>{canny}のように線が非常に多くなってしまうので、あまりロイ・リキテンスタインっぽくはならなさそうです。
//image[canny][Canny法による輪郭抽出の例@<raw>{|latex|\protect}@<fn>{canny}][scale=0.7]
//footnote[canny][@<raw>{|latex|\protect}@<href>{https://en.wikipedia.org/wiki/Canny_edge_detector}より引用]

そこで、今回はNYU Depth Dataset v2@<bib>{nathan}というデータセットを使うことにします。
これは、Kinectで撮影した室内風景の写真とその深度情報を組み合わせたデータセットで、約41万枚の画像が含まれています。
そして、そのうちの1449枚には人間によって付与された物体の領域情報がラベルとして含まれています。
実際にデータを見てみると@<img>{nyudepth}のように、少しロイ・リキテンスタインのような雰囲気が見て取れるのが分かります。
//image[nyudepth][NYU Depth Dataset v2に含まれる写真と深度情報、そして物体の領域情報の例@<raw>{|latex|\protect}@<fn>{nyudepth}]
//footnote[nyudepth][@<raw>{|latex|\protect}@<href>{http://cs.nyu.edu/~silberman/datasets/nyu_depth_v2.html}より引用]

では、このデータセットの写真を絵画風に変換するスクリプトをPythonで実装していきたいと思います。
データセットはMatlab向けの.matファイルなのでh5pyで読み込みます。
また、実行時引数としてデータセットのファイルのパスと、写真・変換された画像を保存するディレクトリを受け取ります。

//emlistnum[データセットの読み込み][python]{
if __name__ == '__main__':
    if len(sys.argv) < 4:
        print '%s [mat file] [dir to save original] [dir to save converted]'
        sys.exit(-1)

    mat = h5py.File(sys.argv[1])

    for index, (image, label) in enumerate(zip(mat['images'], mat['labels'])):
        orig, conv = convert(image, label)
        orig.save(os.path.join(sys.argv[2], '%05d.png' % index))
        conv.save(os.path.join(sys.argv[3], '%05d.png' % index))
//}

@<code>{convert(image, label)}はデータセット内の写真と対応するラベルの情報を受け取りますが、
どちらもnumpyの配列形式となっていることに注意する必要があります。
また、ラベルについては、写真にある物体ごとに1以上の番号が割り当てられており、
ピクセルのそれぞれについて、物体の領域に含まれている場合はその番号が、そうでない場合は0が割り当てられているという形式になっています。

//emlistnum[画像の変換][python]{
def convert(image, label):
    orig = Image.fromarray(image.T, 'RGB')

    for index in numpy.unique(label):
        # 物体ごとに中央値をちょっと明るくした色で埋める
        color = tuple([min(255, int(1.3 * numpy.median(image[channel][label == index])))
                                                                for channel in range(3)])

        # テクスチャを生成する
        if (label == index).sum() < image.shape[1] * image.shape[2] * 0.1:
            texture = random.choice([fill, dot, diagonal])(image.shape[1:3], color)
        else:
            if sum(color) / 3 < 128:
                color = tuple([255 - c for c in color])
            texture = fill(image.shape[1:3], color)

        # 物体の輪郭線を書く
        contour = numpy.zeros(label.shape, dtype = numpy.uint8)
        contour[label == index] = 255
        contour = Image.fromarray(contour.T, 'P').convert('RGB')
        contour = numpy.asarray(contour.filter(ImageFilter.CONTOUR)).T

        # テクスチャと輪郭線を載せる
        for channel in range(3):
            image[channel][label == index] = texture[channel][label == index]
            image[channel] = numpy.minimum(image[channel], contour[channel])

    conv = Image.fromarray(image.T, 'RGB')

    return orig, conv
//}

まず、4行目のforによって、画像に含まれる物体のそれぞれについて同じ処理を繰り返すようにしています。
そして、ロイ・リキテンスタインの絵画では物体がそれぞれ1色で塗られていることから、6~7行目で物体を塗りつぶす色を決定しています。
ここでは、物体の領域に含まれる色の中央値を取った上で、原色に近い色にするためにRGBをそれぞれ1.3倍するという処理を行っています。

そこから、9~15行目で決定した色のテクスチャをランダムに生成しています。
ここで、@<code>{fill(shape, color)}は与えられたサイズを与えられた色で埋めた画像を生成する関数、
@<code>{dot(shape, color)}は水玉で埋めた画像を生成する関数、@<code>{diagonal(shape, color)}は斜線で埋めた画像を生成する関数とします。
つまり、後で物体に該当する部分だけ切り抜いて使うために、画像サイズと同じテクスチャを作っておくという処理になっています。
また、ロイ・リキテンスタインの絵画を見ると、壁や床などの大きな領域では幾何学的なテクスチャがあまり用いられていないことから、
物体の領域が画像全体の10%以下の場合のみに水玉や斜線を用いるようにしています。

そして、18~21行目で物体の輪郭線を生成しています。
ここでは、物体の領域だけを白く塗った白黒画像を作った上で、PILの@<code>{ImageFilter.CONTOUR}というフィルタ機能によって輪郭線を得ています。
最後に、24~27行目でRGBのそれぞれについて、物体の領域に該当するテクスチャを切り抜いて貼り付け、
さらにその上から輪郭線を載せるという処理を行っています。

テクスチャを生成する関数は、以下の通りとなっています。
ここについての詳細な説明は割愛しますが、水玉のサイズや斜線の傾きはランダムに決定されるようになっており、描画にはPILの@<code>{ImageDraw}を使っています。

//emlistnum[テクスチャの生成][python]{
def fill(size, color):
    img = Image.new('RGB', size, color)
    return numpy.asarray(img).T

def dot(size, color):
    radius = random.randint(3, 5)
    interval = random.randint(radius + 2, int(radius * 1.7))

    img = Image.new('RGB', size, (255, 255, 255))
    draw = ImageDraw.Draw(img)

    for x in range(0, size[0], interval):
        for y in range(0, size[1], interval):
            y += interval * (x / interval % 2) / 2
            draw.ellipse((x, y, x + radius, y + radius), color)

    return numpy.asarray(img).T

def diagonal(size, color):
    width = random.randint(2, 4)
    angle = random.randint(1, 89)

    img = Image.new('RGB', size, (255, 255, 255))
    draw = ImageDraw.Draw(img)

    for x in range(0, size[0], width * 2):
        y = x / math.tan(angle / math.pi)
        draw.line((x, 0, 0, y), color, width)

    for y in range(width * 2 - size[0] % (width * 2), size[1], width * 2):
        x = size[0] - (size[1] - y) * math.tan(angle / math.pi)
        draw.line((size[0], y, x, size[1]), color, width)

    return numpy.asarray(img).T
//}

以上を実行すると、@<img>{royconv}のように写真とロイ・リキテンスタイン風の変換画像のデータセットが生成できます。
それなりにポップアートっぽい雰囲気になってきた気がします。
//image[royconv][元の写真と生成されたロイ・リキテンスタイン風の変換画像の例]

ここまでのソースコードを@<code>{convert.py}として、コマンドラインの操作をまとめると、以下のようになります@<fn>{shuf}。
1449枚のデータセットのうち、20%にあたる290枚をランダムに選び、テストデータとして学習データには含めず、
学習がうまく進んでいるかを確かめるために使うようにしています。
//footnote[shuf][Macではshufコマンドの代わりに、HomeBrewのcoreutilsに含まれるgshufを使用してください。]

//cmd{
$ mkdir orig label
$ python convert.py nyu_depth_v2_labeled.mat orig label
$ test=`seq 0 1448 | shuf | head -n 290 | xargs printf "%05d.png\n"`
$ for dir in orig label
> do
>     mkdir $dir/train $dir/test
>     for file in $test; do mv $dir/$file $dir/test; done
>     mv $dir/*.png $dir/train
> done
//}

そして、変換したいロイ・リキテンスタインのポップアートもデータセットに加えておきます。
そのために以下のソースコードを用いて、まず@<m>{640 \times 480}の解像度で切り出します。

//emlistnum[ポップアートの切り出し][python]{
import os
import sys
from PIL import Image

target = (640, 480)
interval = 20

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print '%s [input image] [output image]' % sys.argv[0]
        sys.exit(-1)

    img = Image.open(sys.argv[1])
    ratio = max([float(target[i]) / img.size[i] for i in range(2)])
    size = tuple(map(lambda x: int(x * ratio), img.size))
    img = img.resize(size, Image.ANTIALIAS)

    name, ext = os.path.splitext(sys.argv[2])
    index = 0

    for x in range(0, (size[0] - target[0]) / 2 + 1, interval):
        for y in range(0, (size[1] - target[1]) / 2 + 1, interval):
            crop = img.crop((x, y, x + target[0], y + target[1]))
            crop.save('%s_%02d%s' % (name, index, ext))
            index += 1

//}

これを@<code>{crop.py}として以下のような操作を行います。
なお、これらは学習に用いず、また正解データも存在しないので
@<code>{orig}と@<code>{label}に同じものをコピーしておきます。

//cmd{
$ mkdir orig/val label/val
$ for file in `ls lichtenstein`; do python crop.py lichtenstein/$file orig/val/$file; done
$ cp orig/val/* label/val/
//}

また、pix2pixでは入力画像と出力画像を左右に並べたものを学習に使用しますので、
最後にGitHubの実装に含まれている@<code>{combine_A_and_B.py}によって結合処理をしておく必要があります。
出来上がった@<code>{poparts}ディレクトリをpix2pixの@<code>{dataset}以下に配置すれば完成です。

//cmd{
$ mkdir poparts
$ python combine_A_and_B.py --fold_A orig --fold_B label --fold_AB poparts
//}

===[column] 記事中で用いている画像について

次章で変換結果の画像をいくつか掲載していますが、印刷版ではすべて白黒となってしまうために、
それぞれの違いがよくわからないというケースもあるかと思います。
つきましては、@<href>{https://goo.gl/2x7Iet}に本記事で用いた画像の一部を
すべてアップロードしておりますので、適宜ご参照ください。

===[/column]

== 学習させる

それでは、早速学習させていきましょう。
以下のようにしてpix2pixのREADMEに従って学習させ、テストデータを変換してみます。
ちなみに@<code>{train.lua}はデフォルトで200 epochの学習を行うようになっており、
手元のGTX 970では12時間ほど掛かりました。

//cmd{
$ DATA_ROOT=./datasets/poparts name=poparts which_direction=BtoA th train.lua
$ DATA_ROOT=./datasets/poparts name=poparts which_direction=BtoA phase=test th test.lua
$ DATA_ROOT=./datasets/poparts name=poparts which_direction=BtoA phase=val th test.lua
//}

結果は@<img>{rescolor}の通りで、左側のテストデータでの出力画像を見ると、それなりに元の写真に近いものが出力されていることが分かります。
一番下の出力画像ではソファーになぜか扉のような模様が付いてしまっていますが、4つとも遠目で見ると写真っぽく見えなくもないでしょう。
//image[rescolor][200 epoch時点での変換結果@<raw>{|latex|\protect}@<fn>{poparts}][scale=0.98]
//footnote[poparts][使用したロイ・リキテンスタインの絵画は、上から順に@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=90&year=93&work_id=1649}、@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=90&year=90&work_id=3747}、@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=90&year=92&work_id=1482}、@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=90&year=91&work_id=1340}より引用]

一方で、右側のポップアートでの変換結果を見てみると、なんだか入力画像をぼやっとさせただけ画像が出力されてしまっています。
どうやら、もともとの色使いがかなり出力画像にも残ってしまっているために、あまり写真には見えないという結果となってしまいました。
そこで、入力画像をすべて白黒化して学習させるということを試してみました。
単なる白黒写真のカラー化であればpix2pixは有効であると分かっていますし、白黒にしてしまえばポップアートの鮮やかな色使いにもあまり影響されずに済みそうです。
//image[resgray][入力画像を白黒化した場合の200 epoch時点での変換結果][scale=0.98]

@<img>{resgray}の通り、左側のテストデータは白黒化しなかった時に比べ、
色が違っていたり、変な模様が付いていたりと精度が落ちたように見えますが、
右側のポップアートは先程よりかなり写真らしくなっているように見えます。
@<img>{rescolor}とよく見比べてみると、ベッドのシーツが水色からクリーム色になっていたり、
壁が赤色から茶色になっていたりと、それらしい色に置き換えられているのが分かります。
ただ、まだ画像がぼやっとしているような印象を受けるので、60時間ほど掛けて、さらに1000 epochほど学習させてみました。
その結果が@<img>{result}です。
//image[result][入力画像を白黒化した場合の1200 epoch時点での変換結果][scale=0.98]

== おわりに

ここまでお付き合い頂き、ありがとうございました。
もともとは、WaveNet@<bib>{aaron}を使ったネタを書きたいと思っていたのですが、学習がかなり重くデータセットを1周するのに40時間以上掛かる始末だったので、
どうしようかと途方に暮れていた時にpix2pixが発表され、こうした記事を書くに至りました。
ただpix2pixも、論文を読むと、ピクセル単位で対応が取れるような変換を対象としたアルゴリズムであるということが分かりましたが@<fn>{landscape}、
そういった変換のうち白黒写真のカラー化や航空写真からの地図の生成というような面白そうなテーマはすでにサンプルとして扱われており、どういう問題をネタにするのかが難しいところでした。
//footnote[landscape][風景写真とその中央を拡大したものとの変換を試してみた人がいるみたいですが、あまりうまく学習できていないようです。(@<raw>{|latex|\protect}@<href>{http://liipetti.net/erratic/2016/11/25/imaginary-landscapes-using-pix2pix/}参照)]

そんな時に思いついたのが、このロイ・リキテンスタインの絵画を写真風に変換するというネタでした。
写真を絵画風にするシステムはたくさんありますが、絵画を写真風にするというのは聞いたことがないので、正直うまくいくのかわかりませんでしたが、
実際にチャレンジしてみると思った以上にそれらしい結果が得られたので、我ながらびっくりしました。
また、pix2pixの実装がとても使いやすく作られており、すんなりと実験できたのも、大変ありがたかったです。

次回こそは、音声+深層学習ネタで記事を書ければと思っておりますので、ご期待ください。
また、質問などありましたら、@hiromu1996かhiromu1996[at]gmail.comまでご連絡ください。
