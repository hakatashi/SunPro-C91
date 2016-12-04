= 深層学習で芸術家の脳内を再現する

//raw[|latex|\\chapterauthor{hiromu}]

== はじめに

こんにちは。SunProメンバーのhiromu(@hiromu1996)です。
今回は、2016年11月下旬に発表された「pix2pix」@<bib>{phillip}を使って、
絵から芸術家の脳内を再現するということに挑戦してみました。
どういうことかというと、アメリカン・コミックのような画風で知られる、
ポップアートの代表的な画家、ロイ・リキテンスタインの絵画から
彼が脳内で思い浮かべていたであろう風景を再現するシステムを作ってみました。

ロイ・リキテンスタインといっても、あんまりイメージがないかもしれませんが、
代表作の1つである@<img>{hairribbon}の「ヘアリボンの少女」は見たことがある人もいるんじゃないかと思います。
今回は、そうしたリキテンスタインの作品の中でも、
@<img>{interior}のように室内の風景をモデルにした「インテリア」シリーズを対象とします。
//image[hairribbon][Roy Lichtenstein「Girl with Hair Ribbon」(1965)@<raw>{|latex|\protect}@<fn>{hairribbon}][scale=0.5]
//footnote[hairribbon][@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=60&year=65&work_id=278}より引用]
//image[interior][Roy Lichtenstein「Large Interior with Three Reflections」(1993)@<raw>{|latex|\protect}@<fn>{interior}][scale=0.9]
//footnote[interior][@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=90&year=93&work_id=1649}より引用]

== pix2pixとは

まずは、pix2pixについて、その概要を説明していきたいと思います。
pix2pixは、画像から画像への変換を必要とする問題に対して、包括的に使用できる手法として提案されており、
@<bib>{phillip}では、@<img>{pix2pix}のように、白黒写真のカラー化から、航空写真の地図への変換、あるいは、
線画からの画像の復元にまで適用できることが示されています。
//image[pix2pix][pix2pixの適用例(@<bib>{phillip}より引用)]

pix2pixは、深層学習の中でも「敵対的生成ネットワーク(Generative Adverserial Network, GAN)」という技術を基にしています。
GANでは、乱数から画像を生成するGeneratorと、学習データとGeneratorが生成した画像を見分けるDiscriminatorという
2つのニューラルネットワークを競争させながら学習することで、学習データに似たような画像を自動生成することができるようになっています。
例えるなら、Generatorは贋作画家、Discriminatorは鑑定士で、@<img>{gan}のように互いに勝負を繰り返させることで、
贋作画家の画力は少しずつ向上し、鑑定士の能力も高くなっていくという仕組みになっています。
//image[gan][敵対的生成ネットワーク(GAN)のイメージ図][scale=0.9]

pix2pixは、これを発展させた「条件付き敵対的生成ネットワーク(Conditional GAN)」という技術を使用しています。
Conditional GANでは、航空写真と地図、白黒写真とカラー写真のように入力と出力に対応する2つの画像のペアを学習データと用います。
そして、Generatorには、航空写真や白黒写真といった、入力側となる学習データを与え、その画像に合った画像を生成するようにします。
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
これは、生成された画像をピクセル単位で正解に近づけるような制約となり、GANでの生成結果がよりリアルになることが知られています@<bib>{deepak}。

さらに、pix2pixでは、PatchGANと呼ばれる技術を導入しています。
これは、Discriminatorが判定をする際に、画像全体を与えるのではなく@<m>{16 \times 16}や@<m>{70 \times 70}といった小さな領域ごとに区切って与えるという手法で、
画像を大域的に見て判定するというDiscriminatorの機能を失うことなく、パラメータ数を削減することができるため、より効率的に学習を進められるとしています。

そして、pix2pixの実装は、すべてGitHub(@<href>{https://github.com/phillipi/pix2pix})に公開されています。
つまり、ペアとなった画像の学習データを用意すれば、ここまで説明してきた技術をすぐに使えるというわけです。
こんな素晴らしいものを使わない手はないですよね。

#@# あとがきに、画像を絵画風にするシステムはたくさんあるけど、絵画を画像にするシステムはないということを書く
#@# あとがきに、ピクセル単位で対応が取れて、かつ誰もやってなさそうなテーマを考えたときに思いついたことを書く
