# To-Do List for Symplicity

Features:
* Filter contour
* Sustain pedal support
* Pitch wheel support
* Mod wheel support (vibrato and/or filter)
* Full support for parameters (saving and loading)

Rewrites:
* Redo the entire interface system

Fixes / changes:
* Add an indicator of the current key
* Fix the envelope sometimes not decaying for short notes (jumping straight to release)
*** Idea: maybe it has something to do with if the release is triggered before the attack finishes, and the progression to decay never happens because time is negative at that point...
