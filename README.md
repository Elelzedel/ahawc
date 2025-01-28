# AHAWC

## Overview

This is all of the code that runs my well controller. It is split into "high" and "low" code.

### High

This is Rust code that runs on a Raspberry Pi and handles notifications and basic well switching logic.

## Low

The low code is firmware written in C++ to run on an Arduino Nano Every and controls pump protection and tank level detection. It communicates with the high code on the Raspberry Pi over serial.
