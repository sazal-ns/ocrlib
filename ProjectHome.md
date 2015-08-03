Dear friends!
One year more our group has been developing OCR (optical character
recognition) and translation system with Open Source code for Asian languages.
So it is 10 years now:) wow!
> At present it is OCR more then million pages for www.tbrc.org and www.dharmabook.ru libraries with support of Trace Foundation www.trace.org, St.Petersburg State University and Moscow Dharma Center Rime community members.

The key features of the OCR system include:

1. At present it is MacOS server version Tibetan, Sanskrit, Sinhala, Kannada, Latin and Cyrilic OCR.

2. High accuracy
For Tibetan books, the current recognition results are 1-3 error per 1000
characters. It is include dictionary and mixed text.
On next stages of development it is need same error level on manuscripts and damaged text OCR

3. Fast low-end database with associative search on base of Markov-chain algorithm
1gb/sec search with fuzzy query

4. Tibetan spell checker and dictionary on 359 000 words and 8.5
mln phrase list.

5. Poor resolution and damaged texts recognition. At present system OCR 72 dpi scanned texts. In development algorithm for hand write and xylographic text.

6. PHP and HTML based user interface. At present it is MacOS 10.8-10.9
Local version is Cocoa+PHP.

For OCR you can send us your texts for OCR
www.dharmabook.ru/ocr/ - it is free as a WIND:)

ANY IDEA? = PLEASE MAIL US gomde (at) mail.ru
we will help you to start use a program and source for your project.
ALL SOURCE AND MACOS DMG(12Gb) www.dharmabook.ru/ocrlib


Best regards,
YagpoOCR team
gomde (at) mail.ru
Rime Center Russia
OCR Project Web pages:
www.buddism.ru/ocrlib

Tibetan OCR
![http://www.dharmabook.ru/ocrlib/Yagpo!OCR.png](http://www.dharmabook.ru/ocrlib/Yagpo!OCR.png)

Kannada OCR
![http://www.buddism.ru/ocrlib/KannadaOCR.png](http://www.buddism.ru/ocrlib/KannadaOCR.png)

Credit: His Hollines Gyalwa Karmapa XVI. His Hollines Gyalwa Karmapa XVII. His Hollines Sakya Tridzin. His Hollines Bogdo Gegen Rinpoche. His Hollines Kyabje Tsetrul Rinpoche.
Founding from Trace Foundation. Main partner Tibetan Buddhist Research Center.

FAQ.

---

Why no TESSERACT?
Tesseract it is 20 year old JB2 algorithm. OCRLib is neocognitron with program letter feature definition.
We need OCR more than 1mln volumes of tibetan damaged manuscripts.

---

How build?
Source in XCode 4 for MacOS all frameworks from standard MacOS
RegExp and XML parser included in source.
Any problem? ->gomde#mail.ru

---

How use?
General OCR documentation http://www.buddism.ru/ocrlib/documentation/
Best way send text to us for OCR www.dharmabook.ru
Hard way it is read documentation, source headers and ask us - all is documented in header files.
Mainly OCRLib you can use with any interface which will send request in form of command line argument.
So it can be PHP, Perl, sell script, Apache as CGI interface, Cocoa.
At default if has no input command line - start Cocoa GUI.
Main crossroad for all command input - processInput.cpp
All command data store on global inputData.

---

How train new language or font?
Best way - give it to us. Training is complicated. Mainly training it is from TTF font or scanned book. From font it is more easy - it is need font and alphabet list. From book it is need letter mark-up.

---

Why is so HUGE?
We has server version and no individual users.

---

If you need  small OCR engine - gomde#mali.ru
Main core of OCRLib it is 3.5 mb program. If you no need dictionary and need small letterbase - it can be really small.

---

Which image processing function in OCRLib?
1. Cocoa image format handling - JPG, TIF, GIF, PNG, PDF
2. Adaptive level, adaptive binarisation, page screw correction, split pages, split columns, border cleaning, gaussian blur.

---

Which output format?
RTF, XML, TXT, HTML


