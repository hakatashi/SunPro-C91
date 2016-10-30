# SunPro会誌 2017 開発用リポジトリ

[![Build Status][travis-image]][travis-url]

[travis-image]: https://travis-ci.com/hakatashi/SunPro-C91.svg?token=LBP6dMS3oXazpwBS3Fws&branch=master
[travis-url]: https://travis-ci.com/hakatashi/SunPro-C91

## 開発方針

**1次締切は11月28日(月)24:00(JST)です**

* ブランチは切らずmasterにガンガンpushしてください。
    * `--force` はダメ
* 新規記事を書き始める場合は、
    1. 新しく`(自分のid).re`を作成し、
    1. catarog.ymlのCHAPSの欄にそれを追加し、(順番は後で考えます)
    1. config.ymlのautの欄に自分のidを追加してください。
* ひとり1章です。複数ファイルへの分割も避けてください。
    * 前回の冬コミの時のような肥大化を避けるため、なるべくコンパクトな内容を心がけてください。
    * 書くのが辛くなってきたらあとからERB化とか考えます。
* コミットメッセージはそんなに気にしなくていいです。英語か日本語でお願いします。
* 挿絵はimagesディレクトリの下に自分のidと同じフォルダを作成し、その中に置いてください。
    * 書籍版では白黒で印刷されることに注意してください。
* このリポジトリは初版発行から半年後に公開する予定なので公開できない情報をコミットしないでください。

## 手元でビルドするには

手元での確認はあまり重要ではないので確認無しでガンガンコミットすれば良いと思いますが、
手元で事前に確認したい場合は以下の手順でお願いします。

### 必要なもの

Ruby 2.3.1

### インストール

Bundlerをインストール

    gem install bundler

依存gemをインストール

    bundle install

### ビルド

    bundle exec rake all
