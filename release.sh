#!/usr/bin/env bash

# Create release directory
echo "Copying files..."
rm -r release
mkdir release
mkdir release/prog2-beleg
cp -r doc/ release/prog2-beleg/
cp -r include/ release/prog2-beleg/
cp -r src/ release/prog2-beleg/
cp -r tests/ release/prog2-beleg/
cp .gitignore CMakeLists.txt README.md README.de release/prog2-beleg/

# Add copyright header to all source and include files
cd release/prog2-beleg
find . -name \*.h -o -name \*.cpp|while read fname; do
  cat doc/copyright-header.txt $fname > $fname.tmp
  mv $fname.tmp $fname
done
cd ../../

# Render documentation
echo "Rendering documentation..."
cd release/prog2-beleg/
pandoc -c doc/markdown.css -t html5 README.de -o README.de.html
wkhtmltopdf -B 20 -T 20 -L 20 -R 20 README.de.html README.de.pdf
rm README.de.html
mv README.de.pdf ../
cd ../../

# Package
echo "Packaging..."
cd release
tar -cvzf prog2-beleg.tar.gz prog2-beleg
cd ..

echo "Done!"
