import requests

artists = ['HONOKA', 'ELI', 'KOTORI', 'UMI', 'RIN', 'MAKI', 'NOZOMI', 'HANAYO', 'NICO']
songs = [
    ['もぎゅっと"love"で接近中!', 'もぎゅっとloveで接近中'],
    ['愛してるばんざーい!', '愛してるばんざーい'],
    ['Wonderful Rush', 'Wonderful-Rush'],
    ['Oh,Love&Peace!', 'Oh-Love-and-Peace'],
    ['僕らは今のなかで', '僕らは今のなかで'],
    ['WILD STARS', 'WILD-STARS'],
    ['きっと青春が聞こえる', 'きっと青春が聞こえる'],
    ['輝夜の城で踊りたい', '輝夜の城で踊りたい'],
    ['Wonder zone', 'Wonder-zone'],
    ['START:DASH!!', 'START-DASH']
]

def download(url, filename):
    res = requests.get(url, timeout=10)
    with open(filename, 'wb') as f:
        f.write(res.content)

timeouts = []

for artist in artists:
    for song in songs:
        print('Downloading: %s(%s Mix)' % (song[0], artist))
        baseurl = 'https://itunes.apple.com/search'
        params = {
            'term': '%s(%s Mix)' % (song[0], artist),
            'country': 'JP',
            'media': 'music',
            'entry': 'song'
        }
        res = requests.get(baseurl, params=params)
        previewUrl = res.json()['results'][0]['previewUrl']
        try:
            download(previewUrl, '%s-%s.m4a' % (song[1], artist))
        except requests.exceptions.ReadTimeout:
            print('Timeout: %s(%s Mix)' % (song[0], artist))
            timeouts.append({
                'url': previewUrl,
                'song': song,
                'artist': artist
            })

for timeout in timeouts:
    print('Downloading again: %s(%s Mix)' % (timeout['song'][0], timeout['artist']))
    try:
        download(timeout['url'], '%s-%s.m4a' % (timeout['song'][1], timeout['artist']))
    except requests.exceptions.ReadTimeout:
        print('Timeout again, please download manually: ' + timeout['url'])
