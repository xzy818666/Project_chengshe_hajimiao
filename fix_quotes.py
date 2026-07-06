import re

with open('tutorialdialog.cpp', 'r', encoding='utf-8') as f:
    content = f.read()

# Fix the double-quote issue at line 96
content = content.replace('通胀。</p>""', '通胀。</p>"')

with open('tutorialdialog.cpp', 'w', encoding='utf-8') as f:
    f.write(content)

print("Fixed!")
