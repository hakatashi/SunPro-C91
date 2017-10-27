set -x

rake web
cp -R images/ web/images/
cd web/
npm install
npm run build
