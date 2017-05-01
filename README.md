## Synopsis

obsgetter is the test out project assigned for CS235. It is a command line application that retrieves weather reports from [National Weather Service](http://w1.weather.gov).

## Building
Running "make" will generate the obsgetter application. Running "make doc" will generate a Word DOCX document from the markdown SDD.

## Installation

Not currently an available target. The executable is intended to be run from the local directory. The path itself doesn't matter, but it needs a weather.cfg available within the folder to know how to output the weather information.

## Tests

None currently. As no license is chosen yet, I could not find a decent header only C unit test library to utilize. Implementing my own would have required more effort than I could afford. Testing is ad-hoc and manual.

## License

MIT License

Copyright (c) 2017 Nick Wade

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
