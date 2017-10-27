const config = require('./base-config.js');
const fs = require('fs');

Object.assign(config.config, {
	author: 'hiromu',
	description: 'こんにちは。SunProメンバーのhiromu (@hiromu1996) です。今回は、2016年11月下旬に発表された「pix2pix」を使って、絵から芸術家の脳内を再現するということに挑戦してみました。一体どういうことなのかというと、アメリカン・コミックのような画風で知られる、ポップアートの代表的な画家、ロイ・リキテンスタインの絵画から彼が脳内で思い浮かべていたであろう風景を再現するシステムを作ってみた、という記事となります。ロイ・リキテンスタインといっても、あんまりイメージが浮かばないかもしれませんが、代表作の1つである図2.1の「ヘアリボンの少女」は見たことがある人もいるんじゃないかと思います。今回は、そうしたリキテンスタインの作品の中でも、図2.2のように室内の風景をモデルにした「インテリア」シリーズを対象とします。'
});

config.page = {
	url: 'hiromu.html',
	plain_title: '深層学習で芸術家の脳内を再現する',
	title: '深層学習で<wbr>芸術家の<wbr>脳内を<wbr>再現する',
	subtitle: 'hiromu',
    content: fs.readFileSync('../webroot/hiromu.html'),
    pdf: {
      link: 'hiromu.pdf',
      size: 15.1,
    },
    prev: {
      link: 'hideo54.html',
      title: '機械学習でμ’s の声を識別する',
    },
    next: {
      link: 'hakatashi.html',
      title: 'ISUCON 2016 参戦記',
    },
};

module.exports = config;
