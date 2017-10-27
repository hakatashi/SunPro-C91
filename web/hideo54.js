const config = require('./base-config.js');
const fs = require('fs');

Object.assign(config.config, {
	author: 'hideo54',
	description: 'こんにちは、hideo54です。灘高校で高校2年生をしています。最近の口癖は「受験したくない」です。これまでずっとSunPro会誌に記事を出してきましたが、もしかすると高校生でSunPro会誌の記事を執筆するのはこれが最後かもしれない……?! さて、僕は今年1月のNHKでの再放送をきっかけとして今更ラブライブ！にハマってしまったわけですが、この記事ではタイトル通り、μ\'sのメンバーの声を学習し、誰が歌っているのか判別できるようにした話をします。'
});

config.page = {
	url: 'hideo54.html',
	plain_title: '機械学習でμ’sの声を識別する',
	title: '機械学習で<wbr>μ’sの声を<wbr>識別する',
	subtitle: 'hideo54',
    content: fs.readFileSync('../webroot/hideo54.html'),
    pdf: {
      link: 'hideo54.pdf',
      size: 0.6,
    },
    next: {
      link: 'hiromu.html',
      title: '深層学習で芸術家の脳内を再現する',
    },
	og_image: 'images/hideo54/a.png',
};

module.exports = config;
