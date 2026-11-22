UCAHAL (Universal Cross-Arch Hardware Abstraction Layer)

Scope:
The UCAHAL is a project whose main goal is to make a standard HAL for every architecture, 
to make OS Development easier and beginner friendly. 
This project is being licensed under MIT license.
Currently UCAHAL supports RISC-V 64-bit architecture. And still in Work In Progress state.
UCAHAL's goal is to make an universal HAL to be able to work with any type of the architecture. 
No matter if it's RV-64 or x86_32 or other architecture.


UCAHAL provides:
Unified API (hal_api.h): Single entry point for Hardware and Kernel interaction (HaKI)
Architectural abstraction layer: Handles architecture-specific registers and instruction into a simple abstracted functions.
Plug-and-play Design: Easy build and integration (Just combine .a file of HAL with the .a file of your kernel)

UCAHAL includes:
System topology: Unified structures. Such as memory_map, boot_info and etc.
Core control: Basic CPU management including core initialization,core ID retrieval, and IPI (WIP)
Early Console: A standartized text output,various data formats
Block I/O: Generic disk interface based on block reading/writing (WIP)
Support of different BSP (for RV64 right now, WIP too)





THIS PROJECT IS ON WORK IN PROGRESS (W.I.P.) STATE!

copyright (c) 2026 Main Developer of an UCAHAL