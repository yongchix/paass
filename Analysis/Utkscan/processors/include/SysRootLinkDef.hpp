///@file SysRootLinkDef.hpp
///@brief SysRootLinkDef for Various ROOT output structures
///@authors T.T. King, R. Yokoyama
///@date 2/6/2018

#ifndef PAASS_LINKDEF_HPP
#define PAASS_LINKDEF_HPP
#ifdef __CINT__

//#pragma link C++ struct processor_struct::CLOVERS+;
//#pragma link C++ class std::vector<processor_struct::CLOVERS>+;

//#pragma link C++ struct processor_struct::DOUBLEBETA+;
//#pragma link C++ class std::vector<processor_struct::DOUBLEBETA>+;

#pragma link C++ struct processor_struct::GAMMASCINT+;
#pragma link C++ class std::vector<processor_struct::GAMMASCINT>+;

#pragma link C++ struct processor_struct::LOGIC+;
#pragma link C++ class std::vector<processor_struct::LOGIC>+;

//#pragma link C++ struct processor_struct::PSPMT+;
//#pragma link C++ class std::vector<processor_struct::PSPMT>+;

// added by YX
#pragma link C++ struct processor_struct::YAP+;
#pragma link C++ class std::vector<processor_struct::YAP>+;
// added by YX
#pragma link C++ struct processor_struct::JAEA2015+;
#pragma link C++ class std::vector<processor_struct::JAEA2015>+;

//#pragma link C++ struct processor_struct::ROOTDEV+;
//#pragma link C++ class std::vector<processor_struct::ROOTDEV>+;

//#pragma link C++ struct processor_struct::VANDLES+;
//#pragma link C++ class std::vector<processor_struct::VANDLES>+;

#pragma link C++ class PixTreeEvent+;
#pragma link C++ class std::vector<PixTreeEvent>+;
#endif

#endif //PAASS_LINKDEF_HPP
