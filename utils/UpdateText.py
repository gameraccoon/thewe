from zipfile import ZipFile
import re

def fix_identical_compression(text):
	fixedText = text
	# exclude empty cells
	badResult = re.compile('<table\:table\-cell\s+table\:number\-columns\-repeated\=\"\d+\"(?!>)*?/>')
	fixedText = badResult.sub("", fixedText)

	r = re.compile('<table\:table\-cell\s+table\:number\-columns\-repeated\=\"(\d+)\".*?>.*?</table\:table\-cell>')
	while r.search(fixedText):
		res = r.search(fixedText, 1)
		groups = res.groups()
		unresult = re.compile('\stable\:number\-columns\-repeated\=\"\d+\"')
		fixedGroup = unresult.sub("", res.group())
		fixedText = r.sub(fixedGroup * int(groups[0]), fixedText, 1)

	return fixedText

def fix_issues(fileForFix):
	content = []
	with open(fileForFix) as f:
		content = f.read()

	# fix double line breaks issue
	content = content.replace('<text:p/>', '<text:p></text:p>')

	#fix compression of identical texts issue
	content = fix_identical_compression(content)

	with open(fileForFix, 'w') as f:
		f.write(content)

# unpacking localizations
with open('../Resources/texts/content.xml', 'w') as f:
	with ZipFile('../UISources/Localizations.ods', 'r') as z:
		f.write(z.read('content.xml'))

# fix some issues
fix_issues("../Resources/texts/content.xml")
