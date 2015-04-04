#ifdef USE_REQUIREDELEMENTS
if (pkgName == "req")
{
	switch (sb->getTypeCode())
	{
		case SBML_MODEL:
		case SBML_COMPARTMENT:
		case SBML_COMPARTMENT_TYPE:
		case SBML_CONSTRAINT:
		case SBML_EVENT:
		case SBML_EVENT_ASSIGNMENT:
		case SBML_FUNCTION_DEFINITION:
		case SBML_INITIAL_ASSIGNMENT:
		case SBML_KINETIC_LAW:
		case SBML_LIST_OF:
		case SBML_PARAMETER:
		case SBML_REACTION:
		case SBML_RULE:
		case SBML_SPECIES:
		case SBML_SPECIES_REFERENCE:
		case SBML_SPECIES_TYPE:
		case SBML_MODIFIER_SPECIES_REFERENCE:
		case SBML_UNIT_DEFINITION:
		case SBML_UNIT:
		case SBML_ALGEBRAIC_RULE:
		case SBML_ASSIGNMENT_RULE:
		case SBML_RATE_RULE:
		case SBML_SPECIES_CONCENTRATION_RULE:
		case SBML_COMPARTMENT_VOLUME_RULE:
		case SBML_PARAMETER_RULE:
		case SBML_TRIGGER:
		case SBML_DELAY:
		case SBML_STOICHIOMETRY_MATH:
		case SBML_LOCAL_PARAMETER:
			return SWIGTYPE_p_ReqSBasePlugin;
	}
}

#endif // USE_REQUIREDELEMENTS 
