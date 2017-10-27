const config = require('./base-config.js');
const fs = require('fs');

Object.assign(config.config, {
	author: 'hakatashi',
	description: 'まえがきぶりの博多市です。本当はもうちょっとウィットに富んだ記事を執筆する予定だったのですが、残念ながら個人的な事情でまとまった時間が取れなかったため、執筆できませんでした。が、個人的に書く意欲はあるので、4月9日に開催が決まった技術書典2にもし受かったらそこで蔵出ししたいと思います。というわけで、今回はSunProのメンバーでISUCON6に参加したときの記録を綴りたいと思います。予選参加以前の模擬試合から本戦までの記録をわりと綿密に書いたので参戦記にしては長くなってしまいましたが、そのぶん読んでためになる文章になっていると思います。今後ISUCONに参加する人もそうでない人も、ぜひぜひご覧ください。'
});

config.page = {
	url: 'hakatashi.html',
	plain_title: 'ISUCON 2016 参戦記',
	title: 'ISUCON <wbr>2016 <wbr>参戦記',
	subtitle: 'hakatashi',
    content: fs.readFileSync('../webroot/hakatashi.html'),
    pdf: {
      link: 'hakatashi.pdf',
      size: 0.8,
    },
    prev: {
      link: 'hiromu.html',
      title: '深層学習で芸術家の脳内を再現する',
    },
};

module.exports = config;
