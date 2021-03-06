(* 

category:      Test
synopsis:      Basic single reaction involving two species and a non-unity
                  stoichiometry.
componentTags: Compartment, Species, Reaction, Parameter 
testTags:      Amount, NonUnityStoichiometry
testType:      TimeCourse
levels:        1.2, 2.1, 2.2, 2.3, 2.4, 2.5, 3.1, 3.2
generatedBy:   Analytic

The model contains one compartment called "compartment".  There are two
species called S1 and S2 and one parameter called k1.  The model contains
one reaction defined as:

[{width:30em,margin: 1em auto}|  *Reaction*  |  *Rate*  |
| S1 -> 2 S2 | $k1 * S1 * compartment$  |]
 
Note the stoichiometry of S2 is 2.

The initial conditions are as follows:

[{width:30em,margin: 1em auto}|       |*Value*         |*Units*     |
|Initial amount of S1                |$1.5 \x 10^-2$ |mole        |
|Initial amount of S2                |$0$             |mole        |
|Value of parameter k1               |$1$             |second^-1^  |
|Volume of compartment "compartment" |$1$             |litre       |]

The species' values are given as amounts of substance to make it easier to
use the model in a discrete stochastic simulator, but (as per usual SBML
principles) their symbols represent their values in concentration units
where they appear in expressions.

Note: The test data for this model was generated from an analytical
solution of the system of equations.

*)

newcase[ "00003" ];

addCompartment[ compartment, size -> 1 ];
addSpecies[ S1, initialAmount -> 1.5 10^-2 ];
addSpecies[ S2, initialAmount -> 0 ];
addParameter[ k1, value -> 1 ];
addReaction[ S1 -> 2 S2, reversible -> False,
	     kineticLaw -> k1 * S1 * compartment ];

makemodel[]

