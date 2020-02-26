This example bot only chases the ball however the possibilities are endless with brainfuck

Packet inputs
cell 0: bot location x
cell 1: bot location y
cell 2: bot velocity x
cell 3: bot velocity y
cell 4: ball location x
cell 5: ball location y

Controller outputs
cell 6: throttle
cell 7: steer
cell 8: boost
cell 9: handbrake

Working space
cell 10: A
cell 11: B
cell 12: C
cell 13: D
cell 14: E
cell 15: F

, > , > , > , > , > ,   get all inputs

> + >> + <<<<<<<<       throttle = 1 and boost = 1


dot = (
    bot_vel_y * (ball_loc_x subtract bot_loc_x)
    subtract
    bot_vel_x * (ball_loc_y subtract bot_loc_y)
)



Calculate ball_loc_x sub bot_loc_x and store in A

Non destructive copy of bot_loc_x to A with B as temp
[>>>>> >>>>>+ >+
 <<<<< <<<<<  <-]
>>>>> >>>>> >           go to B
[<<<<< <<<<< <+
 >>>>> >>>>> >-]        loop ends at B

Non destructive copy of ball_loc_x to B with C as temp
<<<<< <<                go to ball_loc_x
[>>>>> >>+ >+
 <<<<< <<  <-]
>>>>> >>>               go to C
[<<<<< <<<+
 >>>>> >>>-]            loop ends at C

ball_loc_x sub bot_loc_x (B sub A)
<<                      go to A
[-> -<]                 B sub A
> [<+ >-]               move B contents to A
                        loop ends B



Calculate ball_loc_y sub bot_loc_y and store in B

Non destructive copy of bot_loc_y to B with C as temp
<<<<< <<<<<             go to bot_loc_y
[>>>>> >>>>>+ >+
 <<<<< <<<<< <-]        loop ends at bot_loc_y
>>>>> >>>>> >           go to C
[<<<<< <<<<< <+
 >>>>> >>>>> >-]        loop ends at C

Non destructive copy of ball_loc_y to C with D as temp
<<<<< <<                go to ball_loc_y
[>>>>> >>+ >+
 <<<<< <<<-]
>>>>> >>>               go to D
[<<<<< <<<+
 >>>>> >>>-]            loop ends at D

ball_loc_y sub bot_loc_y (C sub B)
<<                      go to B
[-> -<]                 C sub B
> [<+ >-]               move C contents to B
                        loop ends C


Move bot velocity x and y to C and D

Non destructive copy of bot_vel_x to C with D as temp
<<<<< <<<<<             go to bot_vel_x

[>>>>> >>>>>+ >+
 <<<<< <<<<<  <-]       move bot_vel_x to C and D
                        loop ends at bot_vel_x
>>>>> >>>>> >           go to D
[<<<<< <<<<< <+
 >>>>> >>>>> >-]        move D to bot_vel_x
                        loop ends at D

Non destructive copy of bot_vel_y to D with E as temp
<<<<< <<<<<             go to bot_vel_y
[>>>>> >>>>>+ >+
 <<<<< <<<<<  <-]       move bot_vel_y to D and E
                        loop ends at bot_vel_y
>>>>> >>>>> >           go to E
[<<<<< <<<<< <+
 >>>>> >>>>> >-]        move E to bot_vel_y
                        loop ends at E


So far:
A: ball_loc_x sub bot_loc_x
B: ball_loc_y sub bot_loc_y
C: bot_vel_x
D: bot_vel_y


Multiply bot_vel_x by ball_loc_y sub bot_loc_y (C * B) and store in B
E and F are temp
<<<                     go to B
[>>>>+ <<<<-]           move B to F
>>>>                    go to F
[
  <<<                   go to C
  [<+ >>>+ <<-]         move C to both B and E
                        loop ends at C
  >>                    go to E
  [<<+ >>-]             move E to C
                        loop ends at E
  >-                    sub 1 from F
]                       loop ends at F

Multiply bot_vel_y by ball_loc_x sub bot_loc_x (D * A) and store in A
E and F are temp
<<<<<                   go to A
[>>>>>+ <<<<<-]         move A to F
>>>>>                   go to F

[
  <<                    go to D
  [<<<+ >>>>+ <-]       move D to both A and E
                        loop ends at D
  >                     go to E
  [<+ >-]               move E to D
                        loop ends at E
  >-                    sub 1 from F
]                       loop ends at F

ball_loc_y sub bot_loc_y (B sub A)
<<<<<                   go to A
[-> -<]                 B sub A
> [<+ >-]               move B contents to A
                        loop ends B


Check if A is negative

Set B to 127
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Clean up C and D
>[-] >[-]               at D


<<<                     go to A

Add 127 to A
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

C will be 1 if A is positive / 0 if A is negative
[
 >>> +
 <<  [- >>[-] > + <<<]
 >>  [- <+ >]
 >   [- <<<+ >>>]
 <<<- <-
]

>[-]+                   set B to 1 as a flag
>                       at C

[                       if C != 0
  [-]                   clear C
  < [-]                 remove B flag
  <<<< +                set steer to 1 (drive right)
  >>>>>                 go to C
]

<                       at B
[                       if B != 0 it means C is 0
  [-]                   clear B
  <<<< -                set steer to negative 1 (drive left)
  >>>>
]                       loop ends at B


Controller outputs
cell 6: throttle
cell 7: steer

Starting from output 0 controls must be in this order:
steer throttle pitch yaw roll jump boost handbrake

We need to move the outputs from out temporary storage into
the right cells for the outputs

<<<<            go to cell 6 (steer)
.               output steer
<               go to cell 7 (throttle)
.               output throttle
[-]             empty cell for the next outputs
......          output empty values for the rest of the controls