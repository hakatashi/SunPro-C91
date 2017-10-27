module.exports = {
  config: {
    title: 'SunPro 2017',
    author: 'SunPro',
    email: 'admin@sunpro.io',
    description: 'コミックマーケット91にて発行された同人誌『SunPro会誌 2017』です。現在無料公開中。「機械学習でμ’s の声を識別する」「深層学習で芸術家の脳内を再現する」「ISUCON 2016 参戦記」の3本でお届けします。',
    hatena_base: 'http://b.hatena.ne.jp/entry/s/sunpro.io/C91/',
	base: 'https://sunpro.io/C91/',
  },
  page: {
    url: '',
    plain_title: '',
    subtitle: '',
    content: '',
    posts: [],
  },
  theme: {
    'navbar-links': {
      'Home': undefined,
      'GitHub': 'https://github.com/hakatashi/SunPro-C91',
    },
    footer: [],
    avatar: '/../../favicon.png',
    comment: {
      enable: false,
    },
    highlight: {
      enable: true,
    },
    header: {
      title: 'SunPro <wbr>2017',
      motto: '技術系よろず同人誌 (2016年12月発行)',
      bigimgs: [{
        src: '/../../cover.jpg',
        desc: '表紙',
      }],
    },
  },
  url_for: (path) => {
    if (path === undefined) {
      return './';
    } else if (path.startsWith('http')) {
      return path;
    } else {
      return `beautiful-hexo/source${path}`;
    }
  },
};
