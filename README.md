# Piet Interpreter
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](http://choosealicense.com/licenses/mit/)

This is intended to be an interpreter for [the Piet programming language](https://esolangs.org/wiki/Piet "Piet Programming Language esoteric langauge Wiki"). Piet is a stack-based esoteric programming language designed by David Morgan-Mar where programs are bitmap images that look like abstract art (specifically based off [Piet Mondrian](https://en.wikipedia.org/wiki/Piet_Mondrian)).

The inteneded functionality (not in order) of this interpreter is as such:

    1. Execute piet command programs (Note pointer and switch commands will not change order of execution)
    2. Execute piet image programs
    3. Provide debugging functionality to:
       a) command history
       b) step through the program
       c) view the stack as the program executes
       d) view direction pointer and codel chooser directions
       e) view current position in program on image
       f) view previous and next positions and next command in program
    4. Provide interpreting options e.g. define custom behviour for undefined behaviour in language spec
    5. Implement compiler from commands to image
    6. Implement assembler to image (potentially to commands as intermediate step, depending on branch ability due to pointer and switch command incompatability)

# The Piet Language

Piet uses a palette of 20 colours. A pixel in Piet is also called a codel and a collection of adjacent codels is called a block (note the adjecency must be across an edge, diagonals are not valid). A Piet program is run by traversing the blocks of a program using the area of a block as the integer it represents and transitions of colour between blocks as commands.

## The Piet Colour Palette

<table align="center" font color="#000000">
    <tr>
		<td bgcolor="#FFC0C0" align="center" ><font color="#000000">#FFC0C0<br>light red</font></td>
		<td bgcolor="#FFFFC0" align="center"><font color="#000000">#FFFFC0<br>light yellow</font></td>
		<td bgcolor="#C0FFC0" align="center"><font color="#000000">#C0FFC0<br>light green</font></td>
		<td bgcolor="#C0FFFF" align="center"><font color="#000000">#C0FFFF<br>light cyan</font></td>
		<td bgcolor="#C0C0FF" align="center"><font color="#000000">#C0C0FF<br>light blue</font></td>
		<td bgcolor="#FFC0FF" align="center"><font color="#000000">#FFC0FF<br>light magenta</font></td>
	</tr>
	<tr>
		<td bgcolor="#FF0000" align="center"><font color="#000000">#FF0000<br>red</font></td>
		<td bgcolor="#FFFF00" align="center"><font color="#000000">#FFFF00<br>yellow</font></td>
		<td bgcolor="#00FF00" align="center"><font color="#000000">#00FF00<br>green</font></td>
		<td bgcolor="#00FFFF" align="center"><font color="#000000">#00FFFF<br>cyan</font></td>
		<td bgcolor="#0000FF" align="center"><font color="#000000">#0000FF<br>blue</font></td>
		<td bgcolor="#FF00FF" align="center"><font color="#000000">#FF00FF<br>magenta</font></td>
	</tr>
	<tr>
		<td bgcolor="#C00000" align="center"><font color="#000000">#C00000<br>dark red</font></td>
		<td bgcolor="#C0C000" align="center"><font color="#000000">#C0C000<br>dark yellow</font></td>
		<td bgcolor="#00C000" align="center"><font color="#000000">#00C000<br>dark green</font></td>
		<td bgcolor="#00C0C0" align="center"><font color="#000000">#00C0C0<br>dark cyan</font></td>
		<td bgcolor="#0000C0" align="center"><font color="#000000">#0000C0<br>dark blue</font></td>
		<td bgcolor="#C000C0" align="center"><font color="#000000">#C000C0<br>dark magenta</font></td>
	</tr>
	<tr>
		<td bgcolor="#FFFFFF" align="center" colspan="3"><font color="#000000">#FFFFFF&nbsp;white</font></td>
		<td bgcolor="#000000" align="center" colspan="3"><font color="#FFFFFF">#000000&nbsp;black</font></td>
	</tr>
</table>

Table from https://www.dangermouse.net/esoteric/piet.html

These commands are expanded uppon in [Piet Commands](#piet-commands)

The colours of a codel can change in two ways, it can get darker or it can change hue. The table above can be used to find the distance between any two colour (counting down and to the right) and the changes are cyclical (roll over the right edge to the left and top to bottom). For example dark magenta -> light red changes once in hue and once in darkness whereas light red -> dark magenta changes 5 times in hue and twice in darkness.

The combination of hue changes and lightness changes are used to index a table indicating the command to be run:

<table><tr>
		<th>&nbsp;</th><th colspan="3">Lightness change</th>
	</tr>
	<tr>
		<th>Hue change</th>
        <th>None</th>
        <th>1 Darker</th>
        <th>2 Darker</th>
	</tr>
	<tr>
		<th>None</th>
        <td>&nbsp;</td>
        <td>push</td>
        <td>pop</td>
	</tr>
	<tr>
		<th>1 Step</th>
        <td>add</td>
        <td>subtract</td>
        <td>multiply</td>
	</tr>
	<tr>
		<th>2 Steps</th>
        <td>divide</td>
        <td>mod</td>
        <td>not</td>
	</tr>
	<tr>
		<th>3 Steps</th>
        <td>greater</td>
        <td>pointer</td>
        <td>switch</td>
	</tr>
	<tr>
		<th>4 Steps</th>
        <td>duplicate</td>
        <td>roll</td>
        <td>in(number)</td>
	</tr>
	<tr>
		<th>5 Steps</th>
        <td>in(char)</td>
        <td>out(number)</td>
        <td>out(char)</td>
	</tr>
</table>

Table from https://www.dangermouse.net/esoteric/piet.html

## Piet Execution

A Piet interpreter maintains a current pixel, a direction pointer and a codel choser (also a direction). The direction pointer can point in any of the four directions whereas the codel chooser can only point left and right and is relative to the direction pointer.

The interpreter starts at the top left codel of the image with the direction pointer pointing left and the codel chooser pointing pointing left also. 
   * The interpreter find the edge of the current block furthest in the direction of the direction pointer and moves the current pixel to that codel.
   * Then the interpreter finds the codel along the current edge of the current block furthest in the direction of the codel chooser and moves the current pixel to that codel.
   * Finally the interpreter moves from that codel into the codel immediately in the direction of the direction pointer.
      * If the colour is white then the interpreter continues in that direction until it find a different colour (slides across the white).
      * If the colour is black then the interpreter cannot move into that codel, the codel choser is toggled, and the interpreter tries again, if still unable then the direction pointer is rotated clockwise one step. If the interpreter runs full circle then the program terminates.

## Piet Commands

The Piet language consists of 17 commands:

   * push - push the value onto the stack
   * pop - pop the top value off the stack
   * add - pop the top two values off the stack, add them and push the result onto the stack
   * subtract - pop the top two values off the stack, subtract the top value from the second top and push the result onto the stack
   * multiply - pop the top two values off the stack, multiply them and push the result onto the stack
   * divide - pop the top two values off the stack, divide the second top value by the top and push the result onto the stack
   * mod - pop the top two values off the stack, divide the second top value by the top and push the remainder onto the stack
   * not - pop the top value off the stack, push 0 if the value was non 0 otherwise 1
   * greater - pop the top two values off the stack, push 1 if the second top value was greater than the top value otherwise 0
   * pointer - pop the top value off the stack and rotate the direction pointer clockwise by that many steps (if negative counter clockwise)
   * switch - pop the top value off the stack and toggles the codel chooser's direction that many times (ignores sign)
   * duplicate - push a copy of the top value onto the stack
   * roll - pops the top two stack entries n (second top) and k (top) off the stack, rotates the top n entries k times, if k is 1 then this will take the nth value in the stack and bring it to the top of the stack
   * in(number) - reads a value from STDIN, expecting a numerical value
   * in(char) - read a value from STDIN, expecting a character
   * out(number) - pop the top value off the stack and output it as a number
   * out(char) - pop the top value off teh stack and output it as a char (ASCII)

# Credits

The piet programs in this repository where found online at https://www.dangermouse.net/esoteric/piet/samples.html and were created by:

   * piet_pi.png by Richard Mitton (edited by myself to create piet_pi2.png and piet_pi_stop_in_circle.png, and converted to plain text in Example_Program.txt, for testing purposes)
   * piet_HellowWorld.gif by Thomas Schoch
   * piet_tetris_big.png by Anthony Maës
   * hw5.png by Kelly Boothby
   * fizzbuzz.png by Sergei Lewis
   * pietquest.png by Sergei Lewis
   * 99bottles.png by Eddy Ferreira