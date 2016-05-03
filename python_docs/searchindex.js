Search.setIndex({envversion:46,filenames:["accessing_model","api_reference","index","installing_rr","integration","introduction","metabolic","read_write_functions","selecting_values","stability","steady_state","stochastic","stoichiometric","using_roadrunner","utility_functions","what_is_rr","what_is_sbml"],objects:{"":{roadrunner:[1,0,0,"-"]},"RoadRunner.Compiler":{getCompiler:[1,2,1,""],getCompilerLocation:[1,2,1,""],getDefaultTargetTriple:[1,2,1,""],getHostCPUName:[1,2,1,""],getProcessTriple:[1,2,1,""],getSupportCodeFolder:[1,2,1,""],setCompiler:[1,2,1,""],setCompilerLocation:[1,2,1,""],setSupportCodeFolder:[1,2,1,""]},"RoadRunner.Config":{LOADSBMLOPTIONS_CONSERVED_MOIETIES:[1,3,1,""],LOADSBMLOPTIONS_MUTABLE_INITIAL_CONDITIONS:[1,3,1,""],LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION:[1,3,1,""],LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION:[1,3,1,""],LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION:[1,3,1,""],LOADSBMLOPTIONS_OPTIMIZE_GVN:[1,3,1,""],LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING:[1,3,1,""],LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER:[1,3,1,""],LOADSBMLOPTIONS_PERMISSIVE:[1,3,1,""],LOADSBMLOPTIONS_READ_ONLY:[1,3,1,""],LOADSBMLOPTIONS_RECOMPILE:[1,3,1,""],LOADSBMLOPTIONS_USE_MCJIT:[1,3,1,""],MAX_OUTPUT_ROWS:[1,3,1,""],ROADRUNNER_DISABLE_PYTHON_DYNAMIC_PROPERTIES:[1,3,1,""],ROADRUNNER_DISABLE_WARNINGS:[1,3,1,""],getConfigFilePath:[1,4,1,""],readConfigFile:[1,4,1,""],setValue:[1,4,1,""],writeConfigFile:[1,4,1,""]},"RoadRunner.ExecutableModel":{"__getitem__":[1,2,1,""],"__setitem__":[1,2,1,""],getBoundarySpeciesAmounts:[1,2,1,""],getBoundarySpeciesConcentrations:[1,2,1,""],getBoundarySpeciesIds:[1,2,1,""],getCompartmentIds:[1,2,1,""],getCompartmentVolumes:[1,2,1,""],getConservedMoietyIds:[1,2,1,""],getConservedMoietyValues:[1,2,1,""],getDependentFloatingSpeciesIds:[1,2,1,""],getFloatingSpeciesAmountRates:[1,2,1,""],getFloatingSpeciesAmounts:[1,2,1,""],getFloatingSpeciesConcentrations:[1,2,1,""],getFloatingSpeciesIds:[1,2,1,""],getFloatingSpeciesInitAmountIds:[1,2,1,""],getFloatingSpeciesInitAmounts:[1,2,1,""],getFloatingSpeciesInitConcentrationIds:[1,2,1,""],getFloatingSpeciesInitConcentrations:[1,2,1,""],getGlobalParameterIds:[1,2,1,""],getGlobalParameterValues:[1,2,1,""],getIndependentFloatingSpeciesIds:[1,2,1,""],getInfo:[1,2,1,""],getModelName:[1,2,1,""],getNumBoundarySpecies:[1,2,1,""],getNumCompartments:[1,2,1,""],getNumConservedMoieties:[1,2,1,""],getNumEvents:[1,2,1,""],getNumFloatingSpecies:[1,2,1,""],getNumGlobalParameters:[1,2,1,""],getNumRateRules:[1,2,1,""],getNumReactions:[1,2,1,""],getReactionIds:[1,2,1,""],getReactionRates:[1,2,1,""],getStateVector:[1,2,1,""],getStateVectorId:[1,2,1,""],getStateVectorIds:[1,2,1,""],getStateVectorRate:[1,2,1,""],getStoichiometry:[1,2,1,""],getTime:[1,2,1,""],items:[1,2,1,""],keys:[1,2,1,""],reset:[1,2,1,""],resetAll:[1,2,1,""],resetToOrigin:[1,2,1,""],setBoundarySpeciesConcentrations:[1,2,1,""],setCompartmentVolumes:[1,2,1,""],setConservedMoietyValues:[1,2,1,""],setFloatingSpeciesAmounts:[1,2,1,""],setFloatingSpeciesConcentrations:[1,2,1,""],setFloatingSpeciesInitAmounts:[1,2,1,""],setFloatingSpeciesInitConcentrations:[1,2,1,""],setGlobalParameterValues:[1,2,1,""],setTime:[1,2,1,""]},"RoadRunner.LoadSBMLOptions":{conservedMoieties:[1,3,1,""],mutableInitialConditions:[1,3,1,""],noDefaultSelections:[1,3,1,""],readOnly:[1,3,1,""],recompile:[1,3,1,""]},"RoadRunner.Logger":{LOG_CRITICAL:[1,3,1,""],LOG_CURRENT:[1,3,1,""],LOG_DEBUG:[1,3,1,""],LOG_ERROR:[1,3,1,""],LOG_FATAL:[1,3,1,""],LOG_INFORMATION:[1,3,1,""],LOG_NOTICE:[1,3,1,""],LOG_TRACE:[1,3,1,""],LOG_WARNING:[1,3,1,""],disableConsoleLogging:[1,4,1,""],disableFileLogging:[1,4,1,""],disableLogging:[1,4,1,""],enableConsoleLogging:[1,4,1,""],enableFileLogging:[1,4,1,""],getColoredOutput:[1,4,1,""],getCurrentLevelAsString:[1,4,1,""],getFileName:[1,4,1,""],getFormattingPattern:[1,4,1,""],getLevel:[1,4,1,""],levelToString:[1,4,1,""],log:[1,4,1,""],setColoredOutput:[1,4,1,""],setFormattingPattern:[1,4,1,""],setLevel:[1,4,1,""],setProperty:[1,4,1,""],stringToLevel:[1,4,1,""]},"RoadRunner.RoadRunner":{clearModel:[1,2,1,""],conservedMoietyAnalysis:[1,3,1,""],getAvailableIntegrators:[1,2,1,""],getCC:[1,2,1,""],getConfigurationXML:[1,2,1,""],getConservationMatrix:[1,2,1,""],getCurrentSBML:[1,2,1,""],getDiffStepSize:[1,2,1,""],getEE:[1,2,1,""],getEigenValueIds:[1,2,1,""],getExistingIntegratorNames:[1,2,1,""],getExtendedVersionInfo:[1,2,1,""],getFrequencyResponse:[1,2,1,""],getFullEigenValues:[1,2,1,""],getFullJacobian:[1,2,1,""],getFullStoichiometryMatrix:[1,2,1,""],getInfo:[1,2,1,""],getInstanceCount:[1,2,1,""],getInstanceID:[1,2,1,""],getIntegrator:[1,2,1,""],getIntegratorByName:[1,2,1,""],getKMatrix:[1,2,1,""],getL0Matrix:[1,2,1,""],getLinkMatrix:[1,2,1,""],getModel:[1,2,1,""],getNrMatrix:[1,2,1,""],getParamPromotedSBML:[1,2,1,""],getReducedEigenValues:[1,2,1,""],getReducedJacobian:[1,2,1,""],getReducedStoichiometryMatrix:[1,2,1,""],getSBML:[1,2,1,""],getScaledConcentrationControlCoefficientMatrix:[1,2,1,""],getScaledElasticityMatrix:[1,2,1,""],getScaledFloatingSpeciesElasticity:[1,2,1,""],getScaledFluxControlCoefficientMatrix:[1,2,1,""],getSelectedValues:[1,2,1,""],getSteadyStateSolver:[1,2,1,""],getSteadyStateThreshold:[1,2,1,""],getSteadyStateValues:[1,2,1,""],getUnscaledConcentrationControlCoefficientMatrix:[1,2,1,""],getUnscaledElasticityMatrix:[1,2,1,""],getUnscaledFluxControlCoefficientMatrix:[1,2,1,""],getUnscaledParameterElasticity:[1,2,1,""],getUnscaledSpeciesElasticity:[1,2,1,""],getuCC:[1,2,1,""],getuEE:[1,2,1,""],gillespie:[1,2,1,""],integrate:[1,2,1,""],isModelLoaded:[1,2,1,""],load:[1,2,1,""],model:[1,3,1,""],oneStep:[1,2,1,""],plot:[1,5,1,""],reset:[1,2,1,""],resetAll:[1,2,1,""],resetToOrigin:[1,2,1,""],setConfigurationXML:[1,2,1,""],setDiffStepSize:[1,2,1,""],setIntegrator:[1,2,1,""],setIntegratorSetting:[1,2,1,""],setSteadyStateThreshold:[1,2,1,""],simulate:[1,2,1,""],steadyState:[1,2,1,""],steadyStateSelections:[1,3,1,""],steadyStateSolver:[1,1,1,""],steadyStateSolverExists:[1,2,1,""],timeCourseSelections:[1,3,1,""]},"RoadRunner.Roadrunner":{getSimulationData:[1,5,1,""]},"RoadRunner.SelectionRecord":{index:[1,3,1,""],p1:[1,3,1,""],p2:[1,3,1,""],selectionType:[1,3,1,""]},"RoadRunner:.Compiler":{getVersion:[1,2,1,""]},"roadrunner.RoadRunner":{"__init__":[1,2,1,""],createSelection:[1,2,1,""],getCompiler:[1,2,1,""],getIds:[1,2,1,""],getValue:[1,2,1,""]},"roadrunner.SelectionRecord":{"_COMPARTMENT":[1,3,1,""],ALL:[1,3,1,""],ALL_DEPENDENT:[1,3,1,""],ALL_DEPENDENT_AMOUNT:[1,3,1,""],ALL_DEPENDENT_CONCENTRATION:[1,3,1,""],ALL_INDEPENDENT:[1,3,1,""],ALL_INDEPENDENT_AMOUNT:[1,3,1,""],ALL_INDEPENDENT_CONCENTRATION:[1,3,1,""],AMOUNT:[1,3,1,""],BOUNDARY:[1,3,1,""],BOUNDARY_AMOUNT:[1,3,1,""],BOUNDARY_CONCENTRATION:[1,3,1,""],COMPARTMENT:[1,3,1,""],CONCENTRATION:[1,3,1,""],CONSREVED_MOIETY:[1,3,1,""],CONTROL:[1,3,1,""],CURRENT:[1,3,1,""],DEPENDENT:[1,3,1,""],EIGENVALUE:[1,3,1,""],EIGENVALUE_COMPLEX:[1,3,1,""],ELASTICITY:[1,3,1,""],ELEMENT:[1,3,1,""],FLOATING:[1,3,1,""],FLOATING_AMOUNT:[1,3,1,""],FLOATING_AMOUNT_RATE:[1,3,1,""],FLOATING_CONCENTRATION:[1,3,1,""],FLOATING_CONCENTRATION_RATE:[1,3,1,""],GLOBAL_PARAMETER:[1,3,1,""],INDEPENDENT:[1,3,1,""],INITIAL:[1,3,1,""],INITIAL_FLOATING_AMOUNT:[1,3,1,""],INITIAL_FLOATING_CONCENTRATION:[1,3,1,""],RATE:[1,3,1,""],REACTION:[1,3,1,""],REACTION_RATE:[1,3,1,""],STOICHIOMETRY:[1,3,1,""],TIME:[1,3,1,""],UNKNOWN:[1,3,1,""],UNKNOWN_CONCENTRATION:[1,3,1,""],UNKNOWN_ELEMENT:[1,3,1,""],UNSCALED:[1,3,1,""],UNSCALED_CONTROL:[1,3,1,""],UNSCALED_ELASTICITY:[1,3,1,""]},"roadrunner.Solver":{getDescription:[1,2,1,""],getDisplayName:[1,2,1,""],getHint:[1,2,1,""],getName:[1,2,1,""],getNumParams:[1,2,1,""],getParamDesc:[1,2,1,""],getParamHint:[1,2,1,""],getParamName:[1,2,1,""],getSettings:[1,2,1,""],getSettingsRepr:[1,2,1,""],getType:[1,2,1,""],getValue:[1,2,1,""],getValueAsBool:[1,2,1,""],getValueAsChar:[1,2,1,""],getValueAsDouble:[1,2,1,""],getValueAsFloat:[1,2,1,""],getValueAsInt:[1,2,1,""],getValueAsLong:[1,2,1,""],getValueAsString:[1,2,1,""],getValueAsUChar:[1,2,1,""],getValueAsUInt:[1,2,1,""],getValueAsULong:[1,2,1,""],hasValue:[1,2,1,""],resetSettings:[1,2,1,""],setValue:[1,2,1,""],settingsPyDictRepr:[1,2,1,""],toRepr:[1,2,1,""],toString:[1,2,1,""]},"roadrunner.roadrunner":{Solver:[1,1,1,""]},RoadRunner:{Compiler:[1,1,1,""],LoadSBMLOptions:[1,1,1,""]},roadrunner:{RoadRunner:[1,1,1,""],RoadRunner_getExtendedVersionInfo:[1,4,1,""],RoadRunner_getParamPromotedSBML:[1,4,1,""],SelectionRecord:[1,1,1,""],getCopyrightStr:[1,5,1,""],getVersionStr:[1,5,1,""],listTestModels:[1,5,1,""],loadTestModel:[1,5,1,""],validateSBML:[1,5,1,""]}},objnames:{"0":["py","module","Python module"],"1":["py","class","Python class"],"2":["py","method","Python method"],"3":["py","attribute","Python attribute"],"4":["py","staticmethod","Python static method"],"5":["py","function","Python function"]},objtypes:{"0":"py:module","1":"py:class","2":"py:method","3":"py:attribute","4":"py:staticmethod","5":"py:function"},terms:{"0068a7f0":1,"0b01":1,"0b10":1,"0b11":1,"0x101c70a00":15,"10e":10,"13def5f8":1,"234567e":10,"4th":13,"54293397e":1,"99954571e":1,"9999999999999997e":13,"__getitem__":1,"__init__":1,"__repr__":1,"__setitem__":1,"__version__":[13,14],"_compart":1,"_j0":1,"_roadrunn":1,"boolean":1,"case":[1,8],"const":1,"default":[1,4,13,15],"enum":1,"final":[12,15],"import":[1,2,5,11],"int":[1,13],"long":13,"new":[1,8,13],"return":[1,8,9,10,13,14,15],"static":1,"switch":13,"true":[1,13,15],"try":[1,4,8,11],"while":8,abbrevi:1,abl:1,abort:1,about:[1,2,8,13,14],abov:1,absolut:[1,4,13],absolute_toler:[1,4,13],accept:[1,2],accessor:[1,15],accomplish:[1,13],accord:13,accordingli:1,actual:[1,8,14],adapt:13,add:1,addit:[1,11,13,15],addition:13,adjust:4,advac:1,advanc:1,affect:[1,6],after:[1,10,15],again:13,against:13,algorithm:[11,13],all:[1,4,6,8,10,13,15],all_depend:1,all_dependent_amount:1,all_dependent_concentr:1,all_independ:1,all_independent_amount:1,all_independent_concentr:1,alloc:1,allow:[0,1,4,6,13],almost:8,alon:1,along:1,alreadi:1,also:[1,8,11,13,14,15],alter:1,alwai:[1,8,13],amount:[1,4,8,13,15],amt:15,analyz:15,andi:[1,15],ani:[1,2,6,8,13,14,15],anoth:[4,13],anyth:[1,13],apach:15,appear:8,append:8,appli:13,applic:1,appropri:[1,13],approxim:[4,11],arg:1,argument:[1,8,13,14],arrai:[1,2,10,13,15],assembl:1,assig:15,assign:1,associ:1,assum:[6,13],attach:13,attempt:1,attribut:[1,4],automat:[1,13,15],awar:1,axi:13,back:[1,13,15],backslash:13,backward:13,balanc:4,base:[1,4,13,14,15,16],basic:[1,2,5],basicblockpass:1,becaus:[1,6],becom:[1,13],been:[1,4,10,13],befor:[1,4],behavior:1,belong:1,below:[1,13],better:1,between:[4,13],biochem:[9,10],biolog:16,biologi:[15,16],biomd0000000010:13,biomodel:13,bit:1,black:1,block:[1,6,15],blue:1,bool:1,both:[1,13],bound:[1,15],boundary_amount:1,boundary_concentr:1,bracket:[8,13],bring:13,brown:1,build:[1,15],built:[1,13,15],cach:1,calcul:[1,4,10,13,15],call:[1,9,10,13,15],can:[1,8,10,11,12,13,14,15,16],cap:13,capabl:[2,13,15],carlo:11,carri:1,cascad:15,caus:[1,13,15],cell:[15,16],centisecond:1,centuri:1,certain:1,cfg:1,cfgsimplif:1,chain:1,chang:[1,2,5,6,8,10],charact:[1,8,13],check:[1,11],chemic:[1,8,12],child:1,choos:13,chosen:4,clean:15,clear:[1,13],clearmodel:1,clf:13,clog:1,close:1,cm0:[1,13],cm_1:1,code:[1,14],coeffici:[1,6,8],coefficic:6,color:1,column:[1,8,13,15],combin:[1,12,14],come:6,command:13,comment:1,compar:1,compartment1:1,compat:1,complet:[1,2],complex:1,compon:[11,15],comput:[1,11],concentr:[1,6,8,10,12,13,15],conf:1,config:1,conservationanalysi:14,conservedmoieti:1,conservedmoietyanalysi:1,conservedmoietyconvert:1,consid:1,consol:[1,13,15],consreved_moieti:1,constant:[1,13],construct:1,constructor:[1,13],consum:[1,12],contain:[1,13],contemporan:1,content:[1,2,13,15],continu:[1,8],convent:[1,8],converg:13,convers:1,convert:1,copi:[1,13,15],copyresult:1,copyright:[1,2,5],corespond:[1,12],correct:13,correspond:1,cost:15,could:1,coupl:[2,13],cours:[1,2,13,15],cpu:1,cpu_typ:1,crash:1,creat:[1,8],createselect:[1,8],creation:11,critic:1,criticalcolor:1,current:[1,8,11,13,14,15],currenttim:1,custom:[1,13],cvode:[1,4,13],cyan:1,cycl:1,dai:1,darkgrai:1,data:[1,2,5],date:[1,14,15],dce:1,dead:1,deadcodeelimin:1,deadinstelimin:1,deal:[10,11],debug:1,debugcolor:1,dec:14,decid:10,decim:1,decreas:4,def:13,default_compart:1,defin:[1,6,8,9,15],definit:[6,15],delet:8,demonstr:15,denot:1,depend:[1,12,15],deriv:8,derivi:6,describ:[1,6,10,12,13],descript:[1,13],design:1,desir:[1,11],detail:[1,13],determin:[1,8,9],determinist:[1,13],dev:14,dictionari:1,did:1,didn:13,differ:[1,13],differenti:[1,4,9,13],directli:[1,2,8,13],directori:[1,13,14,15],disabl:1,disableconsolelog:1,disablefilelog:1,disablelog:1,displai:[1,8,13,15],distinguish:1,distribut:[13,15],divid:1,docstr:15,document:[1,4,8,13,15],doe:[1,8,13],don:1,done:1,dot:15,doubl:[1,13],down:[1,4,15],download:13,drastic:4,drive:[1,13],driven:1,ds1:13,ds2:13,durat:[1,13],dure:13,dynam:[1,10,12],each:[1,12,13],easi:[1,9,15],easiest:13,easili:[10,13,15],ebi:13,eclips:14,effect:1,effector:6,effici:1,eig:15,eigen:1,eigenimag:8,eigenr:8,eigensystem:15,eigenvalu:[1,8,9],eigenvalue_complex:1,either:[1,8,13],elast:[1,6,8],element:[1,8,15],elimin:1,els:1,empti:1,enabl:1,enablecolor:1,enableconsolelog:1,enablefilelog:1,encount:4,end:[1,13,15],endr:1,engin:[1,2],enter:13,entir:1,environ:1,enzym:6,epoch:1,equal:13,equat:[4,9,10,13,15],error:[1,13],errorcolor:1,escap:13,especi:1,essenti:13,estim:[1,4],etc:[1,8],evalu:[1,4,15],even:[1,8,13],evenli:[1,13],everi:1,evolv:1,exactli:13,exampl:[1,2,5,8,10,13,14],example_system:1,exceed:1,excel:15,except:[1,4,8,13],executablemodel:[1,15],exist:[1,8],experi:2,experiment:1,express:[1,12],extend:[8,13],extens:1,extern:[1,6],fail:13,fals:[1,13,14],fast:1,fatal:1,fatalcolor:1,featur:[1,2,13],feb:1,februari:1,feedback:[1,13],feel:4,fehlberg:13,few:13,fewer:1,field:1,file:[1,15],filenam:1,find:[1,13],fine:[1,4],finit:1,first:[1,4,6,12,13,15],fix:13,flag:[1,13],flexibl:1,floating_amount:1,floating_amount_r:1,floating_concentr:1,floating_concentration_r:1,flux:[1,6,8],folder:14,follow:[0,1,6,7,8,9,10,11,12,13,14],footnot:[2,5],form:[1,4,6,7,8,12,15],format:[1,13],forth:13,forward:[1,13],found:[1,8,10,13,15],four:1,fraction:1,framework:1,fred:1,free:15,freed:1,frequenc:1,frequent:[1,4],from:[1,4,9,11,12,13,14,15],full:[1,9],fulli:13,fundament:[2,5],fundamet:15,futur:1,gain:1,gener:[1,4,8,11,13],genetr:11,get:[1,2,13,14,15],get_data:15,getavailableintegr:1,getboundaryspeciesamount:1,getboundaryspeciesconcentr:1,getboundaryspeciesid:1,getcc:[1,8],getcoloredoutput:1,getcompartmentid:1,getcompartmentvolum:1,getcompil:1,getcompilerloc:1,getconfigfilepath:1,getconfigurationxml:1,getconservationmatrix:1,getconservedmoietyid:1,getconservedmoietyvalu:1,getcopyrightstr:1,getcurrentlevelasstr:1,getcurrentsbml:1,getdefaulttargettripl:1,getdependentfloatingspeciesid:1,getdescript:[1,11,13],getdiffsteps:1,getdisplaynam:[1,13],gete:[1,8],geteigenvalueid:1,getexistingintegratornam:1,getextendedversioninfo:1,getfilenam:1,getfloatingspeciesamount:[1,15],getfloatingspeciesamountr:1,getfloatingspeciesconcentr:1,getfloatingspeciesid:[1,15],getfloatingspeciesinitamount:1,getfloatingspeciesinitamountid:[1,13],getfloatingspeciesinitconcentr:[1,13],getfloatingspeciesinitconcentrationid:[1,13],getfloatingspeciesinitialamountid:1,getfloatingspeciesinitialconcentrationid:1,getformattingpattern:1,getfrequencyrespons:1,getfulleigenvalu:1,getfulljacobian:[1,9,15],getfullstoichiometrymatrix:1,getglobalparameterid:1,getglobalparametervalu:1,gethint:[1,13],gethostcpunam:1,getid:1,getindependentfloatingspeciesid:1,getinfo:[1,14],getinstancecount:1,getinstanceid:1,getintegr:[1,11,13],getintegratorbynam:[1,13],getkmatrix:1,getl0matrix:1,getlevel:1,getlinkmatrix:1,getmodel:[1,15],getmodelnam:1,getnam:1,getnrmatrix:1,getnumboundaryspeci:1,getnumcompart:1,getnumconservedmoieti:1,getnumev:1,getnumfloatingspeci:1,getnumglobalparamet:1,getnumparam:1,getnumraterul:1,getnumreact:1,getparamdesc:1,getparamhint:1,getparamnam:1,getparampromotedsbml:1,getprocesstripl:1,getreactionid:1,getreactionr:1,getreducedeigenvalu:1,getreducedjacobian:[1,9],getreducedstoichiometrymatrix:1,getroadrunn:13,getsbml:1,getscaledconcentrationcontrolcoefficientmatrix:1,getscaledelasticitymatrix:1,getscaledfloatingspecieselast:1,getscaledfluxcontrolcoefficientmatrix:1,getselectedvalu:1,getselectionvalu:1,getset:[1,11,13],getsettingsrepr:1,getsimulationdata:[1,13],getstatevector:1,getstatevectorid:1,getstatevectorr:1,getsteadystatesolv:[1,13],getsteadystatethreshold:1,getsteadystatevalu:[1,10],getstoichiometri:1,getsupportcodefold:1,gettim:1,gettyp:1,getucc:[1,8],getue:[1,8],getunscaledconcentrationcontrolcoefficientmatrix:1,getunscaledelasticitymatrix:1,getunscaledfluxcontrolcoefficientmatrix:1,getunscaledparameterelast:1,getunscaledspecieselast:1,getvalu:[1,8],getvalueasbool:1,getvalueaschar:1,getvalueasdoubl:1,getvalueasfloat:1,getvalueasint:1,getvalueaslong:1,getvalueasstr:1,getvalueasuchar:1,getvalueasuint:1,getvalueasulong:1,getvers:1,getversionstr:[1,14],gillespi:[1,11,13],gilliespi:13,give:[1,13],given:[1,6,8,9,11,13,15],global_paramet:1,gmt:1,good:[1,4],grai:1,green:1,grid:1,group:2,gvn:1,had:1,hand:4,hard:13,harm:1,hash:1,hasvalu:1,have:[1,4,8,13,15],held:1,help:2,herbert:1,here:[1,2,15],higher:1,highest:[1,4],highli:1,hint:[1,13],histor:1,hold:15,home:[1,13,15],host:1,hour:1,how:[1,6,13,15],howev:[1,4,8,13],html:15,http:[1,3,13,15,16],huge:15,ideal:4,ident:[1,11,12,15],identifi:[1,8],ignor:[1,8,15],imaginari:[1,8],immedi:1,implement:1,implicit:13,imposs:4,improv:1,includ:[1,2,13],increas:4,independ:[1,6,10,12],index:[1,2,8],indic:1,individu:8,info:1,infom:1,inform:[0,1,2,8,11,13],informationcolor:1,init:[1,8,13],initial_floating_amount:1,initial_floating_concentr:1,initial_time_step:[1,4,11,13],initialtimestep:1,insert:8,insid:8,inspect:13,instal:2,instanc:[1,11,13],instancemethod:15,instanti:1,instead:[1,8],instruct:[1,3],instructioncombin:1,instructionsimplifi:1,integ:1,integr:[1,2],integratorlisten:1,intend:[1,2],intention:8,interact:[1,15],interchang:12,interfac:15,intern:[1,4,13,15],interpret:[1,8],interv:13,introduc:1,introduct:2,invari:1,invers:8,invok:13,involv:15,ipython:15,ismodelload:1,iso:1,issu:[1,13],item:[1,8],iter:[11,13],ith:9,j0_keq1:8,j4_ks4:8,jac:[9,15],jacobian:[1,9,15],jacobiansteps:1,jan:1,januari:1,just:1,keep:1,kei:[1,11,13],kernel:1,key_nam:1,keyword:[1,13],kind:[6,16],kinet:[1,13],known:11,kutta:13,kwarg:1,label:13,languag:[13,16],larg:[1,4],last:1,later:[1,13],law:1,lazi:15,learn:2,left:1,legaci:1,legend:[1,13,15],len:13,length:1,less:[1,10],let:13,leveltostr:1,lib:[1,15],librari:[1,15],libroarunn:1,libsbml:[1,14],libsbmlvers:1,licenc:[2,5],licens:15,lightblu:1,lightcyan:1,lightgreen:1,lightmagenta:1,lightr:1,like:[1,10,13],likewis:6,limit:1,lin:15,linalg:15,line:1,linear:12,linearli:[1,12],linerali:12,link:[1,12],linux:[1,13],list:[1,8,13,15],listen:1,listtestmodel:1,llvm:[1,15],load:[1,2,5],loadsbmlopt:1,loadsbmloptions_conserved_moieti:1,loadsbmloptions_mutable_initial_condit:1,loadsbmloptions_optimize_cfg_simplif:1,loadsbmloptions_optimize_dead_code_elimin:1,loadsbmloptions_optimize_dead_inst_elimin:1,loadsbmloptions_optimize_gvn:1,loadsbmloptions_optimize_instruction_combin:1,loadsbmloptions_optimize_instruction_simplifi:1,loadsbmloptions_permiss:1,loadsbmloptions_read_onli:1,loadsbmloptions_recompil:1,loadsbmloptions_use_mcjit:1,loadtestmodel:1,loc:1,local:[1,6],localhost:1,locat:[1,14],log_:1,log_crit:1,log_curr:1,log_debug:1,log_error:1,log_fat:1,log_inform:1,log_notic:1,log_trac:1,log_warn:1,logger:1,look:[1,2,15],lot:13,lowest:1,mac:[1,2],machin:16,magenta:1,mai:[1,2,4,8,12,13],main_pag:16,maintain:1,make:1,manag:1,mani:[1,4,13,16],manner:13,manual:2,markup:16,match:1,math:4,mathemat:8,matplotlib:[1,2,13,15],matrix:[1,9,12,15],max_output_row:1,maximum:[1,13],maximum_adams_ord:[1,13],maximum_bdf_ord:[1,13],maximum_iter:13,maximum_num_step:[1,13],maximum_time_step:[1,11,13],maximumnumstep:1,maximumtimestep:1,mcjit:1,mcpu:1,mean:[1,11,13],measur:6,memori:1,merg:1,messag:1,method:0,methodsprovid:14,microsecond:1,microsoft:[14,15],mid:13,midnight:1,might:[1,8,11,13],millisecond:1,mind:1,minimum:1,minimum_damp:13,minimum_time_step:[1,11,13],minimumtimestep:1,minut:1,modelgener:1,modelload:1,modelnam:1,modif:1,modifi:[1,8],modul:[1,2,13,14],mon:1,mondai:1,monitor:1,mont:11,month:1,more:[1,2,8,10,13,16],most:[1,2,4,13,15],msg:1,multi:1,multipl:[1,11,13],multiple_step:[1,13],multistep:1,must:[1,8,13],mutableinitialcondit:1,my_config_fil:1,my_plot:13,myfil:1,mymodel:[2,4,13,15],name:[1,8,13,15],nativ:15,natur:13,ndarrai:1,necessari:1,necessarili:1,need:[1,4,13,15],needless:4,net:12,network:[6,10,12,16],never:1,newtim:1,next:[1,13],nice:1,nleq:13,nnnn:1,node:1,nodefaultselect:1,nomenclatur:8,non:1,none:[1,13],nonneg:11,norm:13,notat:[1,13],note:[1,4,8,13],noth:15,notic:[1,15],noticecolor:1,now:[2,4,13],nullspac:1,number:[1,4,8,11,12,13,15],numberofdecad:1,numberofpoint:1,numer:[1,4,13,15],numpi:[1,2,13,15],object:[1,2,4,5,8,13],obtain:[0,1,6,9,10,13],off:[1,13],often:[1,6,10,13],onc:13,onestep:1,onli:[1,8,11,13],open:[1,15],oper:[1,13,15],operating_system:1,optim:1,orchestr:15,order:[1,8,12,13],ordinari:4,org:[1,3,15,16],origin:[1,8,13],other:[1,4,6,13,15,16],otherwis:[1,8,13],out:[1,13],output:[1,2,5],over:[1,4,13,15],overrid:[1,13],own:15,packag:[1,15],pad:1,page:[1,2,3,15],pair:[1,8],param:1,parameterid:[1,8],parameternam:1,pars:1,part:[1,8],partial:6,particular:[1,8],particularli:1,pass:[1,13,14],path:[1,13],pathwai:16,pattern:1,percent:1,perform:[1,4,13,15],perhap:13,period:4,permit:1,perpos:11,pertubr:6,perturb:6,phase:13,place:1,plain:1,plan:1,pleas:3,plot:[1,2,5,8],poco:1,pocoproject:1,point:[1,13],pointer:15,posit:[1,13],possibl:[1,8,9,11,13],potenti:[1,6],power:1,pramet:15,precis:4,prefer:13,preliminari:2,present:1,previou:[1,13],previous:[1,15],print:[1,8,13,14],prioriti:1,problem:4,process:[1,16],processor:1,produc:[1,12],product:[12,15],progag:6,program:1,project:1,prompt:13,properti:[1,2],protocol:1,provid:[1,2,4,15],pseudo:11,pure:1,pyd:1,pylab:[13,15],python:[1,13,14,15],queu:1,quickli:1,quot:8,rais:[1,8,13],ran:1,random:11,rang:[1,8,13],rank:[1,12],rational:1,reach:[1,4,13],reaction1:[1,13],reaction_r:1,reactionid:[1,8],reactionindex:1,reactionindx:1,read:[1,2],readabl:[1,16],readconfigfil:1,readi:13,readonli:1,real:[1,8,13],realiz:1,recommend:1,recompil:1,record:[1,8],recurs:1,red:1,reduc:[1,12],redund:1,regardless:13,regener:11,regular:1,regularli:3,regulatori:16,rel:[1,4,13],relat:[1,9,12],relative_toler:[1,4,13],remain:[1,12,13],rememb:1,remot:1,remov:1,reorder:1,replac:1,repres:[1,8,10,11,13,15,16],represent:1,reproduc:11,requir:[1,4,13],reset:[1,13],resetal:1,resetset:1,resettoorigin:1,respect:[1,6],respons:[1,6,15],rest:15,restpect:1,result:[1,2],retriev:[1,8,10],retun:1,revisit:1,rfc:1,right:1,rk4:13,rkf45:13,roadrun:15,roadrunner_config:1,roadrunner_disable_python_dynamic_properti:1,roadrunner_disable_warn:1,roadrunner_getextendedversioninfo:1,roadrunner_getparampromotedsbml:1,roardrunn:15,role:4,root:15,routin:[1,13],row:[1,12],run:[1,2,5,11],rung:13,safe:1,sai:[1,13],same:[1,13],sampl:[1,13],satisfi:10,sauro:1,scalar:13,scale:1,scipi:2,scope:1,script:2,search:[1,2],seattl:15,second:[1,13],section:[1,13],sectionrecord:1,see:[1,2,8,13],seed:[11,13],sel:[1,8],selectiontyp:[1,8],self:[13,15],sensit:[6,8],senstiv:6,separ:[1,13],sequenc:[8,11],seri:[1,13],session:15,set:[1,4,8,11,13,14,15],setboundaryspeciesconcentr:1,setcoloredoutput:1,setcompartmentvolum:1,setcompil:1,setcompilerloc:1,setconfigurationxml:1,setconservedmoietyvalu:1,setdiffsteps:1,setfloatingspeciesamount:1,setfloatingspeciesconcentr:1,setfloatingspeciesinitamount:[1,13],setfloatingspeciesinitconcentr:[1,13],setformattingpattern:1,setglobalparametervalu:1,setintegr:[1,11,13],setintegratorset:[1,13],setlevel:1,setproperti:1,setsteadystatethreshold:1,setsupportcodefold:1,settim:1,settingspydictrepr:1,setvalu:[1,13],shape:13,share:1,shop:1,shortli:1,should:[1,4,9,13],show:[2,13,15],sign:1,signal:16,signific:4,similar:15,simpl:[1,2,15],simpli:[8,13,15],simplifi:[1,11,13],simulateopt:1,sinc:[1,15],singl:[1,8,10,13,15],singular:[1,9],site:15,situat:9,siutat:6,size:[1,13],slash:[1,13],slection:8,slightli:13,slow:1,slower:1,small:4,smaller:[1,10,13],smooth:4,solut:[1,10,13],solv:4,some:[1,6,13],someon:[1,8],somogyi:1,sourc:[1,15],space:[1,8,13],span:[1,4],special:1,speciesid:1,speciesindex:1,speciesindx:1,specif:[1,13,14],specifi:[1,4,8,13],spent:4,spredsheet:15,squar:[1,8,10,13],stabil:[2,4],stabl:4,standard:[2,13,14],start:[1,2,13],startfrequ:1,starttim:1,startup:13,statement:15,statevector:1,statevectorr:1,stderr:1,steadyst:[1,10,13],steadystateselect:[1,10],steadystatesolv:1,steadystatesolverexist:1,step:[1,4,6,13],stepsiz:1,stiff:[1,13],stiffer:4,stiochiometri:1,stochast:[1,2],stochiometr:1,stochiometri:[1,12],stochiometrix:1,stoich:8,stop:1,store:[1,8,13],str:[1,13],string:[1,8,13,14,15],stringtolevel:1,structur:[1,12,13,15],structuredresult:[1,13],studi:6,studio:14,style:1,subsequ:1,success:[1,10],successfulli:[1,13],suit:13,suitabl:1,sum:[1,10,12],summar:13,sundial:13,support:[1,8,11,13,14],suppos:15,suppress:1,sure:13,symbol:[1,13],synonym:1,syntax:2,system:[1,4,6,9,10,15,16],tabl:1,take:[1,4,8,15],target:1,task:1,techinqu:15,technic:[2,5],tell:13,templat:1,temporari:14,tendenc:1,term:[1,6,15],termin:[1,15],test:[1,13,15],test_1:[13,15],text:1,textual:1,textur:1,than:[1,13],thei:[1,8],them:1,therefor:[1,10,13,15],thi:[1,2,4,6,8,10,11,13,15],thing:[1,4],third:1,those:[6,15],though:[1,8],thread:1,three:[1,13],threshold:1,through:[1,6,11],thu:1,tight:4,tighter:4,time:[1,2,4,8,10,12,13,15],timecours:[1,13],timecourseselect:[1,8,13],timestep:13,toler:[1,4,13],too:4,top:[1,15],torepr:1,tostr:1,total:4,trace:1,tracecolor:1,transcript:15,treat:[1,8],tripl:1,trivial:1,truncat:1,tue:1,tuesdai:1,tune:[1,4],turn:1,tutori:[2,5],twice:13,two:[1,6,13,15],txt:[1,15],type:[1,8,13,15],typic:[1,8,13,15],ucc:8,uee:8,unchang:6,under:15,underscor:1,undesir:11,unexpect:1,unit:[1,13],univers:15,unix:1,unknown:1,unknown_concentr:1,unknown_el:1,unless:13,unreach:1,unscal:[1,8],unscaled_control:1,unscaled_elast:1,updat:[1,3],upon:1,upper:1,upstream:15,uri:1,uriordocu:1,uriorsbml:1,url:13,usa:15,usag:1,usedb:1,usehz:1,user:[0,1],usual:1,utc:1,util:2,utilz:15,val:1,valid:[1,8],validatesbml:1,valu:[1,2,4],vari:13,variable_step_s:[1,11,13],variableid:1,variablenam:1,variablestep:13,variat:11,varieti:1,variou:[1,6,13],veloc:1,vendor:1,veri:[1,2,4,15],versa:1,version:[1,13,14,15],versionstr_:14,versionstr_bas:14,versionstr_compil:14,versionstr_d:14,versionstr_libsbml:14,via:[1,13],virtual:15,visa:1,visual:14,vmax:1,volum:1,wai:[1,13,15],want:[1,8,13,15],warn:[1,13],warningcolor:1,washington:15,wast:4,websit:3,weekdai:1,weight:13,well:[1,13],were:1,what:[1,2,5,8,10,11,13],whatev:15,when:[1,10,11,13,14,15],whenev:[1,13],where:[1,4,6,8,9,10,12,13],wherea:8,whether:[1,13],which:[1,2,4],white:[1,8],whole:1,wide:8,wiki:16,wikipedia:16,window:[1,2],wise:1,wish:13,without:[1,13,15],word:1,work:[1,13,14],worklist:1,would:[1,10,13],wrapper:1,write:[1,2],writeconfigfil:1,written:1,www:[13,15],xml:[1,2,4,13,15,16],year:1,yellow:1,yield:13,you:[1,2,4,11,13,14,15],your:13,yourself:13,zero:[1,10,13],zone:1},titles:["Accessing the SBML Model Variables","RoadRunner API Reference","libRoadRunner User Guide","Installing RoadRunner","Simulation and Integration","Introduction","Metabolic Control Analysis","SBML Read/Write Functions","Selecting Values","Stability Analysis","Steady State Analysis","Stochastic Simulation","Stoichiometric Analysis","Basic Tutorial","Utility Functions","What is libRoadRunner","What Is SBML?"],titleterms:{"class":1,"float":[0,1],"function":[1,7,14],"import":13,accept:8,access:[0,1],analysi:[1,6,9,10,12],api:1,avail:1,basic:13,boundari:[0,1],chang:13,compart:[0,1],compil:1,condit:[0,1,13],configur:1,conserv:1,control:[1,6],copyright:15,data:13,event:1,exampl:15,experi:8,footnot:15,fundament:15,global:[0,1],group:14,guid:2,indic:2,inform:14,initi:[0,1,13],instal:3,integr:4,introduct:5,jit:1,level:1,libroadrunn:[2,15],licenc:15,load:13,log:1,mac:3,main:1,manual:3,metabol:[1,6],method:[1,6,8,12],misc:1,miscellan:1,model:[0,1,13],moieti:1,object:15,option:1,output:13,overview:2,paramet:[0,1,13],plot:13,preliminari:[4,6,12],properti:8,rate:[0,1],reaction:[0,1],read:7,refer:1,result:8,roadrunn:[1,3,13],rule:1,run:13,sbml:[0,1,7,16],select:[1,8,13],selectionrecord:1,simul:[1,4,8,11,13],solver:[1,13],speci:[0,1],stabil:9,state:[1,10],steadi:[1,10],stochast:11,stoichiometr:[1,12],stoichiometri:1,syntax:8,tabl:2,technic:15,tutori:13,user:2,util:14,valu:8,variabl:[0,1],vector:1,what:[15,16],which:8,window:3,write:7}})