<h1>Soul Collector</h1>

**5/29/2014**: The lighting algorithm has been updated a bit to fix some of the I had with it. I also fixed a couple small bugs. If you'd like to see it as it was at the end of CMPSC483W (software design at the Pennsylvania State University), revert to the git tag "v1.0" and give it a compile.

If you just want to see the game, [here's a video of it](https://www.youtube.com/watch?v=eBNp7TAJF8E), and [here's a windows build](https://straypixels.net/SCollector-Dist.zip).

Original text follows...

======

A video game made for CMPSC483W, software design, taught by Max Fomitchev at The Pennsylvania State University.

The game is a top-down adventure game located in purgatory. You have been smited, the pieces of your soul scattered; you must navigate the labrynthine purgatory and gather the pieces of your soul without becoming a beast.

Soul Collector was designed to be cross platform; for multimedia, we used SFML, which can be compiled and used on any platform. The team developed on Windows using Visual Studio 2012, and only a windows exectuable is available, but a Linux Makefile is available. To use it, one must have installed the SFML, SFGUI and Tmx-Parser libraries. No deployment was tested for Mac OS.

This game was completed in a school semester using C++, <a href="http://www.sfml-dev.org/">SFML</a> and <a href="http://www.mapeditor.org/">Tiled</a> by a team named "The UN", composed of 6 members:

Edward Lu - Project Lead<br\>
David Tracey - Graphics & Design<br\>
Chris Lawyer - Programming & Level Design<br\>
Mujing Wang - Programming<br\>
Mustafa Alareed - Programming<br\>
Sha Huang - Programming

For more information, look at the project specification document, located at "spec/Soul Collector.docx".
