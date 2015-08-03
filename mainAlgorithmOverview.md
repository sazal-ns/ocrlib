The structure of the process of recognizing the images used in the library computer recognizers (OCR) OCRLib
www.buddism.ru/ocrlib
Moscow 2010

In basis of process of Optical Recognition Text (OCR) was used two main principle:

ANN as biological prototype OCR.
F. Uossermen. Neurocomputer technique: Theory and practice.
F. Wasserman, Neural Computer Technics: A Theory and Practice.

Principles optimal processing signal.
Gonorovskiy JS Radio Engineering Circuits and signals.
Gonorovskij Iosif Semenovich Radio Engineering Circuits and Signals. University textbook.

Program Description
Library of Optical Computer Recognition of text implemented at simple C + +, (main development environment Xcode MacOS) and can easily ported at others platform. Program oriented at streaming recognition oriental texts.
Texts in Oriental languages using alphabets of a large number of letters, vertical stacks and complex, and sometimes not known (or lost) grammar. In Oriental texts used is on alphabets in hundreds and thousands letters (in Western alphabets tens letters).
Ligatures can combine a few letters in the vertical and horizontal stacks. Ligatures can combine a few letters in the vertical and horizontal stacks. These alloys may take place in the next line, change the shape of their constituent letters. From point of view of OCR this equivalent increase alphabet to thousands letters.

Program OCR divided at several logical blocks:
1. Image preprocessing.
2. Optical recognition text.
3. Grammatical and vocabulary control.
4. Proofs reading tables.
5. HTML interface and management of client-server in aspect of multi-streaming recognition.


Theoretical description algorithms in work logical blocks
1. Image preprocessing
Image preprocessing performed on known principles such as:
- adaptive threshold (at need segmentation of text)
- definition tilt Text and correction tilt text.
- noise and border removing
- page layout recognition and page split

In result obtained  black and white normalized text image ready to recognition in format CCITT Fax 4.
Biological prototype. If man see text turned for example on 90 degrees that as only he understand that text rotated he too turn head (or text) and easily can read.


2. Optical recognition of text.
Biological prototype OCR ideologically is neural network structure which based at structure of visual cortex cerebral brain. From computer point of view  visual bark cerebral brain it is a structure of elements (different types of neurons) with relatively low speed and  very  high degree of parallelism. Consumption energy all visual bark cerebral brain estimated only several watts. Modern computing structure conversely posses on compared with visual bark cerebral brain not high degree parallelism (all processors perform computation sequentially)  high electric consumption energy and very high speed.
Computation power advanced processors reached level after whom can talk about modeling neural networks, structure where based at structure visual cortex.
Model This Network - neokognitron - showed high efficiency its applications for image recognition.
Algorithms of processing, which used visual bark cerebral brain at processing image unprecedented on efficiency, flexibility, compactness and beauty.
In modern science environment accumulated huge experimental material on operations visual cortex cerebral brain.
So central task of development program of Optical Computer Recognizers of text was chosen method of parallel simulation algorithms implemented in visual cortex cerebral brain in serial type structures.
Effective approach is conservation ideology parallel algorithms implemented in visual cortex cerebral brain  at their implementation in serial type structures . When this have refuse from achieve external biological likelihood and pay in attention only at achievement maximum   accuracy and performance.
As   shows   practice   formal   modeling   algorithms   parallel   structures   in   structures serial   type
not   effectively (for    direct   implementation   neuronet   speed - serious   problem   ).
So   Practical implementation   OCR in code C++ quite not like at neural network in that same time maximally preserving ideology parallel algorithms neural network.

3. Description biological prototype "engine" OCR.
![http://www.buddism.ru/ocrlib/documentation/1_OCR_biological_prototype.png](http://www.buddism.ru/ocrlib/documentation/1_OCR_biological_prototype.png)
> Fig.1

> In basis algorithm recognition is base biological principle - work area ON OFF, shown at Fig. 1. At surface retinal eyes formed a 2 round area consisting of photosensitive cells  rods and cones (in Figure 1 red color). First region this ON (on Figure 1 white color) all cones fall in this region connected to neuron exercising excitement (+). All cones fall in region OFF,
concentric circle described around area ON (on Figure 1 dark color) connected to another neuron exercising inhibition (-). The outputs of excitatory (+) and inhibitory neurons (-) are connected to Summing neuron (with drawn mathematical sign sum) which gives threshold reaction proportional difference lighting central ON and peripheral OFF, areas.
Summing neuron betray maximum reaction at subject ILLUMINATION central area ON, simultaneous NOT ILLUMINATION peripheral area OFF and Indifferenceto what would that nor was lighting out areas ON and OFF.
For maximum reaction importantly that would size light spot exactly match with size central ON area and its situation and brightness was constant on all area ON. On Indeed affairs structure ON OFF is detector or filter (resonator) round light spot uniform illumination. The noise/signal rate in this structure ON OFF is a spatial filter close to optimal, for round light spot uniform illumination.
![http://www.buddism.ru/ocrlib/documentation/fig_05_5.gif](http://www.buddism.ru/ocrlib/documentation/fig_05_5.gif)
> Fig.2
> > It note that at surface retinal eyes formed   great number areas ON OFF that mutually overlap (they centers shifted at several cones) than achieved their invariance to status light spots.
![http://www.buddism.ru/ocrlib/documentation/2_recon_show.gif](http://www.buddism.ru/ocrlib/documentation/2_recon_show.gif)
Fig.3


> Suppose that ON and OFF area have not round, and more complex form for example appropriate top part Figures unit "1" is called her "feature" on Fig. 3.
In this case the structure of ON OFF will be the filter (detector) close to the optimal for one of the features of the numbers "1". In one digit (letter) may be several features. For example appropriate upper average and lower Part 1 ". Sign this part of letters cut of it little rectangle.
Fearures may contact or overlap. Totality more features uniquely defines digit (letter). At Fig. 3 animation schematically shows process recognition of digit "1" on three stages. As seen of figure at each neuronal layer implements different levels generalizations up to make solutions on figure, input layer sensitive to simple images, letter features.
Reaction of next layer is more complex and not dependent (from  tilt and scale of image).
In our case reaction will figure "1". Invariance to features place also achieved application large number areas ON OFF (Form which corresponds form of letter feature) and superimposed on each other (their centers shifted at several cones).

Practical implementation algorithms "engine" OCR.

> At input of block OCR served one black and white normalized picture of text (format CCITT Fax 4) and database consisting of letters model appropriate of alphabet of recognition language (format XML). Letters database contains previously marked at features of each letters.
Every letter has several features (upper part Fig. 4 thin green rectangles). Feature extraction is carried out manually in accordance with the characteristics of letters and is done once a new language. If take advantage of biological analogy that connection letters database - References equivalent instantaneous training neuronet a recognizing these letters. Statistics of such an algorithm makes it possible to create a program of automatic placement of signs.
![http://www.buddism.ru/ocrlib/documentation/3_OCR_letter.gif](http://www.buddism.ru/ocrlib/documentation/3_OCR_letter.gif)
> Fig.4

> Process Recognition specific letters shows at animated gif Fig. 4 (motion of a thin green rectangle from left to right, the bottom line recognizable). Top part figure this base data (on This Figure this one Tibetan letter "la"). Bottom part figure this recognizable line (at This Figurethis one Tibetan The letter "sa" other "la"). In top part figure in bold red square shows letter standard with more apart symptoms (six green rectangles). Black color inside green rectangles this region ON gray color region OFF. In exact under with areas ON OFF signs each letters in base Data formed Masks ON OFF. At every letter created on several steam masks ON OFF, namely they and used at recognition.
> For Recognition specific letters taken its first sign with pair masks ON OFF and superimposed at black - white image letters in line.
Counted total number black pixels ieobrazheniya line caught in mask ON, analogous calculated total number black pixels caught in mask OFF.
Then calculated their difference and divided at number pixels Masks ON (tonnes. E. at area Masks ON). Obtained figure this measure coincidencetrait reference with real picture.
So way measure coincidences measure correlation trait reference and image in line is formula

Correlation = 100 **(Sum\_bit\_ON - Sum\_bit\_OFF) / SUM_mask_ ON.**

Ratio multiply 100 that would coefficient correlation changed from 0 to 100 percent. Area Masks ON, SUM_mask_ ON on physical meaning always not is zero.

If (Sum\_bit\_ON - Sum\_bit\_OFF) <0 then this difference replace zero.

This formula mathematically coincides with formula reaction Adder neuron at Fig. 1. Actually calculated integral on image in within Masks ON and separately in within Masks OFF. Similar integral on Square is spatial filter close to optimum (for Data simple Forms symptoms) m. e. the best with point view relations signal noise.
> Then sign discretely shifts at several pixels to the right Fig. 4 calculated coefficient correlation for new position trait and remembered.If at This position earlier calculated coefficient correlations for example for other letters that memorized most of them. In biological modelthis corresponds mutual slowdown neurons and allocation neuron with most strong reaction (discussed neurons at that comes signal about withone and that same Square retinal eyes).
> Discrete shift to the right continues to those then while coefficient correlation not exceed 50 per cent (Correlation> 50). If this happenedthat shift to the right terminated. At Fig. 4 moving thin green rectangle becomes in red at Correlation> 5 0 (the latter letter in lowerline). Then taken second sign letters of base Data together with coordinates on first actuated trait and calculated coefficient correlationsif he more than 30 percent that taken third sign. So way checked all prienaki, in our case all 6. If at each of all six signs coefficientcorrelation more than 30 percent (Correlation> 3 0) that letter is previously recognized. At Fig. 4 this process shows in Size animation thingreen rectangle becomes in red at Correlation> 3 0. If although would at one trait coefficient correlation less than 30 percent that letter ispreviously not the recognized and continues discrete movement first trait along line m. e. continues further search letters. Final solution onRecognition letters adopted after addition as similar way will updated all letters base data (all alphabet on all line). In Finally, in fieldwhere worked first sign there is set coefficients correlation from different letters of them selected most and corresponding him letter isfinally the recognized and fortified bold red square.

Determination of the optimum location feature
![http://www.buddism.ru/ocrlib/documentation/4_OCR_letter.gif](http://www.buddism.ru/ocrlib/documentation/4_OCR_letter.gif)
> Ris.5
> > For simplification earlier was omitted description mechanism itself alignment signs. In biological model mechanism own mind stirovkicorresponds Permanent movement eyes in small area movement similar "tremor." At very case coefficients correlation computed not in point andin rectangular area scan (size about one - two ten   pixels) where and is maximum coefficient correlation. This maximum is calculation, the coefficient correlation in point. "The result work mechanism scan shows at animated gif Fig. 5. Dark blue and gray color Showing Masks traitwhich by scanning. Black letter in which sought given sign. Red region not matches. Green region matches.
> > Mechanism scan performed with application known mechanism which called: "Search maximum with help LEV in wilderness. "LEV in desert worksalternately on Vertical and Horizontal looking maximum coefficient correlation in nebolshly area (On Indeed affairs this variety gradientmethod Search maximum).




3. Grammatical and vocabulary control.

> Grammatical control this descriptive algorithm Grammar language.
> Vocabulary control this Hash function at several million words and Parts words.


4. Proofs Table in aspect Streaming and Checkmate Recognition text.
> Proofs Table Showing at Fig. 6 in Size Green Table in middle lower part thumbnails (small fragment). Proofs Table this way identifydifficult measurable Analysis Random Error recognition. This a visual way thickening oschibok m. e. it that same most original text butconsisting only of Error and possible Error (red color in the recognized text).
Method of detecting errors
The main method of detecting errors is to check the syntax and vocabulary control. Dictionary is created automatically on the basis
analysis of the body text
All text is not passed grammatical kontorol   going with all the recognized books   automatically impose in Size proofreader tables (sheets)and presented user OCR, for evaluation Quality recognizers and adoption solutions on errors (30 pages in 2000 pages of text).



5. HTML interface and communication of client-server   in aspect Streaming Recognition text.
![http://www.buddism.ru/ocrlib/documentation/5_OCR_Lib.png](http://www.buddism.ru/ocrlib/documentation/5_OCR_Lib.png)
Ris.6

> Interface program made at HTML-based Fig. 6 (left top angle) and client-server architecture associated mechanism sockets.
Graphic port of program is normal WWW browser for example Safari, Chrome and t. d.. This allows remotely manage process NetworkingDistributed Optical Recognition text.
Data necessary for work program stored in base Data consisting of letters - References appropriate Alphabetical recognizers (format XML), atFig. 6 yellow table in center pictures.
Recognized, its text issued in format HTML, he shows in right lower corner. Black this initial recognizable text small glowing frameconvenient for Synchronization letters Recognized and Source text. Recognised Text partitioned different flowers. Blue color - exactlyrecognized text. Red Color - Color probable errors if Errors in text is, that their should search only in red the recognized text. Red colorthis not errors. Green color - intermediate situation between two described above (practical values not has too small probability errors).
On the basis of MacOs raalizovana multiprocessors.



Prospects:

1. Further increase   accuracy recognizers and Mixing need Proof raspoznnannyh texts to zero.
2. Further increase   flexibility and scale Independence recognizers.
3. Increase   velocities a Database Data signs for new languages.
4. Increase   performance.
Itself on himself production problem Streaming and Checkmate recognizers Text requires certain Computing resources. At today cheat (OCR) of the Eastern texts at multiprocessor MAC PRO hundred polutarotysyachestranichnyh Volume takes about week ( mode Remote access). Advanced istransition at more more multiprocessor technology for example konetsformynachaloformyCUDA GPU, codenamed «Fermi» NVIDIA (requires usespecialized Graphics cards for calculations).


DRAWINGS:

1. 1\_OCR\_biological\_prototype.png     Biological prototype   OCR.
2.   fig\_05\_5.gif      Structure of the location of neurons in the retina.
3. 2\_recon\_show.gif Picture with blue Animated gif "1" http://www.ampersant.ru/glaz/
4. 3\_OCR\_letter.gif Animated gif with apart signs - rectangles and process OCR.
5. 4\_OCR\_letter.gif Animated gif showing mechanism samoyustirovki signs.
6. 5\_OCR\_Lib.png Snapshot with HTML interface base data recognition and source text proofreader   tables.


REFERENCES:

Gonorovskiy JS Radio Engineering Circuits and signals.
Gonorovskij Iosif Semenovich Radio Engineering Circuits and Signals.konetsformynachaloformy University textbook
http://publ.lib.ru/ARCHIVES/G/GONOROVSKIY_Iosif_Semenovich/_Gonorovskiy_I._S..html

F. Uossermen. Neurocomputer technique: Theory and practice.
http://www.ampersant.ru/nntech.pdf
F. Wasserman, Neural Computer Technics: A Theory and Practice

Willie K., Dete In. Biology
Willy K, Dete V, Biology

![http://www.buddism.ru/ocrlib/documentation/Kannada_OCR_Screen2.png](http://www.buddism.ru/ocrlib/documentation/Kannada_OCR_Screen2.png)
> (India) Direct OCR without dictionary control.

Buddist Cannon recognition
http://www.buddism.ru///ocrlib/documentation/I1PD9678400054.html