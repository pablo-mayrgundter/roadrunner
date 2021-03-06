(*

category:        Test
synopsis:        An initial assignment with an nary relational element.
componentTags:   InitialAssignment, Parameter
testTags:        InitialValueReassigned
testType:        TimeCourse
levels:          2.2, 2.3, 2.4, 2.5, 3.1, 3.2
generatedBy:     Analytic
packagesPresent: 

 In MathML, it is legal to have 3+ arguments to relational elements such as less than.  This model checks to make sure interpreters notice the third argument, which changes the result of the 'piecewise' test.  This test is based on test 01209, except that it will fail if the interpreter mistakenly checks only the first and last arguments of the 'lt' function.

The model contains:
* 1 parameter (x)
The initial conditions are as follows:

[{width:35em,margin: 1em auto}|       | *Value* | *Constant* |
| Initial value of parameter x | $piecewise(1, 1 < 3 < 2, 3)$ | constant |]

Note: The test data for this model was generated from an analytical
solution of the system of equations.

*)
