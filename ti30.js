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

var exp = "0";
var stored_reg = null; /* user-defined temporary register via STO/RCL/SUM/EXC */

var display = ""; /* line displayed while typing in digits or decimal point */

/*
 * Basically, this lets us check if the current expression in the calculator
 * display is an evaluated constant or numerical text the user is inputting.
 */
var is_evaluated_const = true;

function set_tix_precision(value) {
    "use strict";
 // var figures = 8; /* original TI-30 calculator precision:  0.66666667 */
 // var figures = 10; /* TI-30X extended precision:  0.6666666667 */
    var figures = 15; // maximum accurate precision in JavaScript */

    return Number(
     // Math.round(value + "e" + figures) + "e-" + figures
        value.toFixed(figures)
    );
}
function ti_equal() {
    "use strict";
    var evaluation;

    document.getElementById("memory_debug").innerHTML = exp;
    if (is_evaluated_const === false) {
        exp = "(" + exp + ")";
    }
    is_evaluated_const = true;

    try {
        evaluation = set_tix_precision(eval(exp));
        document.getElementById("last_error").innerHTML = "Execution complete.";
    } catch(err) {
        evaluation = err.name;
        document.getElementById("last_error").innerHTML = err.message;
    }

    document.getElementById("memory_reg").innerHTML = evaluation;
    display = ""; /* Calculator blanks text input line after each evaluation. */
    return;
}

function ti_digit(digit) {
    "use strict";

    display += digit;
    if (is_evaluated_const === true) {
        exp = String(digit);
    } else {
        exp += String(digit);
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
function ti_negate() {
    "use strict";

    display = String(-Number(document.getElementById("memory_reg").innerHTML));
    if (is_evaluated_const === true) {
        exp = "-(" + exp + ")";
    } else {
        exp += "*-1";
    }
    document.getElementById("memory_reg").innerHTML = display;
    display = ""; // 2019.01.20 rjs:  annoying hack...need to rewrite all this
    is_evaluated_const = true;
    return;
}

function ti_pi() {
    "use strict";
    var last_char = exp[exp.length - 1];

    display = String(Math.PI);
    document.getElementById("memory_reg").innerHTML = display;
    if (is_evaluated_const === true) {
        exp = display;
    } else {
        if (last_char >= "0" && last_char <= "9") {
            exp += "*";
        }
        exp += display;
    }
    is_evaluated_const = true;
    display = "";
    return;
}
function ti_e() {
    "use strict";
    var last_char = exp[exp.length - 1];

    display = String(Math.E);
    document.getElementById("memory_reg").innerHTML = display;
    if (is_evaluated_const === true) {
        exp = display;
    } else {
        if (last_char >= "0" && last_char <= "9") {
            exp += "*";
        }
        exp += display;
    }
    is_evaluated_const = true;
    display = "";
    return;
}

function leftParen() {
    "use strict";

    display = "";
    if (is_evaluated_const === true) {
        exp = "(";
    } else {
        exp += "(";
    }
    is_evaluated_const = false;
    return;
}
function rightParen() {
    "use strict";

    display = "";
    if (is_evaluated_const === true) {
        exp = ")";
    } else {
        exp += ")";
    }
    is_evaluated_const = false;
    return;
}

/*
 * Instead of emulating TI-30 clear and off buttons, I prefer we have a
 * backspace button and a reset button.
 */
function ti_clear() {
    "use strict";

    display = "";
    exp = "";
    document.getElementById("memory_reg").innerHTML = "0";

    stored_reg = null;
    document.getElementById("M_register").innerHTML = "";
    return;
}
function backspace() {
    "use strict";

    if (is_evaluated_const === true || display.length < 1 || exp.length < 2) {
        ti_clear();
        return;
    }
    display = display.substring(1);
    exp = exp.substring(1);
    document.getElementById("memory_reg").innerHTML = display;
    return;
}

function ti_store() {
    "use strict";

    stored_reg = Number(display);
    document.getElementById("M_register").innerHTML = "M";

    display = "";
    is_evaluated_const = true;
    return;
}
function ti_recall() {
    "use strict";
    var last_char = exp[exp.length - 1];

    display = String(stored_reg);
    if (stored_reg === null) {
        display = "0";
    }
    document.getElementById("memory_reg").innerHTML = display;

    if (is_evaluated_const === true) {
        exp = display;
    } else if (last_char >= '0' && last_char <= '9') {
        exp = display;
    } else {
        exp += display;
    }
    display = "";
    is_evaluated_const = true;
    return;
}
