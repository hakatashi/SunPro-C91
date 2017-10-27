set -x

authors=(hiromu hakatashi hideo54)
for author in  "${authors[@]}"
do
    rake html re=$author.re
    mv $author.html web/
done
cp -R images/ web/images/
cd web/
npm install
npm run build
