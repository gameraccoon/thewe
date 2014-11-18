from zipfile import ZipFile

with open('../Resources/texts/content.xml', 'w') as f:
	with ZipFile('../UISources/Localizations.ods', 'r') as z:
		f.write(z.read('content.xml'))