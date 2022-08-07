for rn in *.htm; do
mv $rn $rn.old
sed 's/; background-image:  url(images\/bkg.gif); background-repeat: repeat-y; background-position: left top//g' $rn.old > $rn
done