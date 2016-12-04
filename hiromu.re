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
代表作の1つである「ヘアリボンの少女」@<img>{hairribbon}は見たことがある人もいるんじゃないかと思います。
そうしたリキテンスタインの作品の中でも、後期の作品である、
室内の風景をモデルにした「インテリア」シリーズ@<img>{interior}を対象とします。

//image[hairribbon][Roy Lichtenstein「Girl with Hair Ribbon」(1965)@<raw>{|latex|\protect}@<fn>{hairribbon}]
//footnote[hairribbon][@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=60&year=65&work_id=278}より引用]

//image[interior][Roy Lichtenstein「Large Interior with Three Reflections」(1993)@<raw>{|latex|\protect}@<fn>{interior}]
//footnote[interior][@<raw>{|latex|\protect}@<href>{http://www.imageduplicator.com/main.php?decade=90&year=93&work_id=1649}より引用]

