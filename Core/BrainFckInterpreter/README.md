# Brain F*ck Interpreter
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](http://choosealicense.com/licenses/mit/)

This is intended to be an interpreter for [the Brain F*ck programming language](https://esolangs.org/wiki/Brainfuck "Brainf**ck Programming Language esoteric langauge Wiki").
Brain F*ck was designed by Urban Müller, inspired by [FALSE](https://esolangs.org/wiki/FALSE), with the intention to create language with the smallest possible compiler for the Amiga OS.

# Brain F*ck Execution

Brain F*ck operates on an array of memory cells (all intially zero) with a pointer initially pointing at the first cell

# Brain F*ck Commands

<table>
	<tr>
		<th>Command</th>
        <th>Effect</th>
	</tr>
	<tr>
		<th>></th>
        <td>Move the pointer to the right</td>
	</tr>
	<tr>
		<th><</th>
        <td>Move the pointer to the left</td>
	</tr>
	<tr>
		<th>+</th>
        <td>Increment the memory cell at the pointer</td>
	</tr>
	<tr>
		<th>-</th>
        <td>Decrement the memory cell at the pointer</td>
	</tr>
	<tr>
		<th>.</th>
        <td>Output the current memory cell as a char</td>
	</tr>
	<tr>
		<th>,</th>
        <td>Take in a char and store it in the current memory cell</td>
	</tr>
	<tr>
		<th>[</th>
        <td>If the value of the current cell is 0, jump past the matching ]</td>
	</tr>
	<tr>
		<th>]</th>
        <td>If the value of the current cell is not, jump back to the matching [</td>
	</tr>
</table>

Table from https://esolangs.org/wiki/Brainfuck