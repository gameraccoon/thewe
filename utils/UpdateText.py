from zipfile import ZipFile

def fix_issues(fileForFix):
	content = []
	with open(fileForFix) as f:
		content = f.read()

	# fix double line breaks issue
	content = content.replace('<text:p/>', '<text:p></text:p>')

	with open(fileForFix, 'w') as f:
		f.write(content)

# unpacking localizations
with open('../Resources/texts/content.xml', 'w') as f:
	with ZipFile('../UISources/Localizations.ods', 'r') as z:
		f.write(z.read('content.xml'))

# fix some issues
fix_issues("../Resources/texts/content.xml")
