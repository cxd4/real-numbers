/*
 * @licstart	The following is the entire license notice for the
 * JavaScript code in this file.
 *
 * TI-30X Simulator:  subsetted JavaScript port of real numbers calculator
 * Copyright (C) 2019 rjs
 *
 * This program is free software:  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * @licend	The above is the entire license notice for the JavaScript
 * code in this file.
 */

var exp = 0;
var display = ""; /* line displayed while typing in digits or decimal point */

/*
 * Basically, this lets us check if the current expression in the calculator
 * display is an evaluated constant or numerical text the user is inputting.
 */
var is_evaluated_const = true;

function round(value) {
    "use strict";
 // var figures = 8; /* original TI-30 calculator precision:  0.66666667 */
 // var figures = 10; /* TI-30X extended precision:  0.6666666667 */
    var figures = 15; // maximum accurate precision in JavaScript */

    return Number(value.toFixed(figures)); /*
    return Number(
        Math.round(value + "e" + figures) + "e-" + figures
    ); */
}
function ti_equal() {
    "use strict";
    var evaluation;

    document.getElementById("memory_debug").innerHTML = exp;
    exp = "(" + exp + ")"; /* to prevent operator precedence from next input */
    try {
        evaluation = round(eval(exp));
	document.getElementById("last_error").innerHTML = "Execution complete.";
    } catch(err) {
        evaluation = err.name;
	document.getElementById("last_error").innerHTML = err.message;
    }

    document.getElementById("memory_reg").innerHTML = evaluation;
    is_evaluated_const = true;
    display = ""; /* Calculator blanks text input line after each evaluation. */
    return;
}

function ti_digit(digit) {
    "use strict";

    display += digit;
    if (is_evaluated_const === true) {
        exp = digit;
    } else {
        exp += digit;
    }
    document.getElementById("memory_reg").innerHTML = display;
    is_evaluated_const = false;
    return;
}
function ti_decimal() {
    "use strict";

    if (is_evaluated_const === true) {
        display = "0.";
	exp = "0.";
    } else {
        display += ".";
        exp += ".";
    }
    is_evaluated_const = false;
    document.getElementById("memory_reg").innerHTML = display;
    return;
}

function ti_add() {
    "use strict";

    exp += " + ";
    display = "";
    is_evaluated_const = false;
    return;
}
function ti_subtract() {
    "use strict";

    exp += " - ";
    display = "";
    is_evaluated_const = false;
    return;
}
function ti_multiply() {
    "use strict";

    exp += "*";
    display = "";
    is_evaluated_const = false;
    return;
}
function ti_divide() {
    "use strict";

    exp += "/";
    display = "";
    is_evaluated_const = false;
    return;
}
