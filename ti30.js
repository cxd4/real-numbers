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

/*
 * Basically, this lets us check if the current expression in the calculator
 * display is an evaluated constant or numerical text the user is inputting.
 */
var is_evaluated_const = true;

function ti_equal() {
    "use strict";
    var evaluation;

    document.getElementById("memory_debug").innerHTML = exp;
    try {
        evaluation = eval(exp);
	document.getElementById("last_error").innerHTML = "Execution complete.";
    } catch(err) {
        evaluation = err.name;
	document.getElementById("last_error").innerHTML = err.message;
    }

    document.getElementById("memory_reg").innerHTML = evaluation;
    is_evaluated_const = true;
    return;
}

function ti_digit(digit) {
    "use strict";

    if (is_evaluated_const === false) {
        exp += digit;
    } else {
        exp = "" + digit;
    }
    document.getElementById("memory_reg").innerHTML = exp;
    is_evaluated_const = false;
    return;
}
function ti_decimal(digit) {
    "use strict";

    if (is_evaluated_const === false) {
        exp += ".";
    } else {
        exp = "0.";
        is_evaluated_const = false;
    }
    document.getElementById("memory_reg").innerHTML = exp;
    return;
}
