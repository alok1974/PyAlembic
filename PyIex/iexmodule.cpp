///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2001-2011, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <pybind11/pybind11.h>
namespace py = pybind11;


#include <Iex.h>
#include <IexErrnoExc.h>
#include <iostream>


using namespace IEX_NAMESPACE;

namespace PyIex {

namespace {

void
testCxxExceptions(int i)
{
    //
    // This function is only for testing.
    // It exercises the PY_TRY / PY_CATCH macros
    // and the C++ to Python exception translation.
    //


    switch (i)
    {
    case 1:
        throw int(1);

    case 2:
        throw std::invalid_argument("2");

    case 3:
        throw IEX_NAMESPACE::BaseExc("3");

    case 4:
        throw IEX_NAMESPACE::ArgExc("4");

    default:
        ;
    }
}

std::string
testBaseExcString(const BaseExc &exc)
{
    return exc.what();
}

std::string
testArgExcString(const ArgExc &exc)
{
    return exc.what();
}

BaseExc
testMakeBaseExc(const std::string &s)
{
    return BaseExc(s);
}

ArgExc
testMakeArgExc(const std::string &s)
{
    return ArgExc(s);
}

} // namespace

} // namespace PyIex

using namespace PyIex;


PYBIND11_MODULE(iex, m){
    using namespace IEX_NAMESPACE;

    m.def("testCxxExceptions", &testCxxExceptions);
    m.def("testBaseExcString", &testBaseExcString);
    m.def("testArgExcString", &testArgExcString);
    m.def("testMakeBaseExc", &testMakeBaseExc);
    m.def("testMakeArgExc", &testMakeArgExc);

    static py::exception<BaseExc> PyBaseExc(m, "BaseExc");

    static py::exception<ArgExc> PyArgExc(m, "ArgExc", PyBaseExc.ptr());
    static py::exception<LogicExc> PyLogicExc(m, "LogicExc", PyBaseExc.ptr());
    static py::exception<InputExc> PyInputExc(m, "InputExc", PyBaseExc.ptr());
    static py::exception<IoExc> PyIoExc(m, "IoExc", PyBaseExc.ptr());
    static py::exception<MathExc> PyMathExc(m, "MathExc", PyBaseExc.ptr());
    static py::exception<NoImplExc> PyNoImplExc(m, "NoImplExc", PyBaseExc.ptr());
    static py::exception<NullExc> PyNullExc(m, "NullExc", PyBaseExc.ptr());
    static py::exception<TypeExc> PyTypeExc(m, "TypeExc", PyBaseExc.ptr());
    static py::exception<ErrnoExc> PyErrnoExc(m, "ErrnoExc", PyBaseExc.ptr());

    static py::exception<EpermExc> PyEpermExc(m, "EpermExc", PyErrnoExc.ptr());
    static py::exception<EnoentExc> PyEnoentExc(m, "EnoentExc", PyErrnoExc.ptr());
    static py::exception<EsrchExc> PyEsrchExc(m, "EsrchExc", PyErrnoExc.ptr());
    static py::exception<EintrExc> PyEintrExc(m, "EintrExc", PyErrnoExc.ptr());
    static py::exception<EioExc> PyEioExc(m, "EioExc", PyErrnoExc.ptr());
    static py::exception<EnxioExc> PyEnxioExc(m, "EnxioExc", PyErrnoExc.ptr());
    static py::exception<E2bigExc> PyE2bigExc(m, "E2bigExc", PyErrnoExc.ptr());
    static py::exception<EnoexecExc> PyEnoexecExc(m, "EnoexecExc", PyErrnoExc.ptr());
    static py::exception<EbadfExc> PyEbadfExc(m, "EbadfExc", PyErrnoExc.ptr());
    static py::exception<EchildExc> PyEchildExc(m, "EchildExc", PyErrnoExc.ptr());
    static py::exception<EagainExc> PyEagainExc(m, "EagainExc", PyErrnoExc.ptr());
    static py::exception<EnomemExc> PyEnomemExc(m, "EnomemExc", PyErrnoExc.ptr());
    static py::exception<EaccesExc> PyEaccesExc(m, "EaccesExc", PyErrnoExc.ptr());
    static py::exception<EfaultExc> PyEfaultExc(m, "EfaultExc", PyErrnoExc.ptr());
    static py::exception<EnotblkExc> PyEnotblkExc(m, "EnotblkExc" , PyErrnoExc.ptr());
    static py::exception<EbusyExc> PyEbusyExc(m, "EbusyExc" , PyErrnoExc.ptr());
    static py::exception<EexistExc> PyEexistExc(m, "EexistExc" , PyErrnoExc.ptr());
    static py::exception<ExdevExc> PyExdevExc(m, "ExdevExc" , PyErrnoExc.ptr());
    static py::exception<EnodevExc> PyEnodevExc(m, "EnodevExc" , PyErrnoExc.ptr());
    static py::exception<EnotdirExc> PyEnotdirExc(m, "EnotdirExc" , PyErrnoExc.ptr());
    static py::exception<EisdirExc> PyEisdirExc(m, "EisdirExc" , PyErrnoExc.ptr());
    static py::exception<EinvalExc> PyEinvalExc(m, "EinvalExc" , PyErrnoExc.ptr());
    static py::exception<EnfileExc> PyEnfileExc(m, "EnfileExc" , PyErrnoExc.ptr());
    static py::exception<EmfileExc> PyEmfileExc(m, "EmfileExc" , PyErrnoExc.ptr());
    static py::exception<EnottyExc> PyEnottyExc(m, "EnottyExc" , PyErrnoExc.ptr());
    static py::exception<EtxtbsyExc> PyEtxtbsyExc(m, "EtxtbsyExc" , PyErrnoExc.ptr());
    static py::exception<EfbigExc> PyEfbigExc(m, "EfbigExc" , PyErrnoExc.ptr());
    static py::exception<EnospcExc> PyEnospcExc(m, "EnospcExc" , PyErrnoExc.ptr());
    static py::exception<EspipeExc> PyEspipeExc(m, "EspipeExc" , PyErrnoExc.ptr());
    static py::exception<ErofsExc> PyErofsExc(m, "ErofsExc" , PyErrnoExc.ptr());
    static py::exception<EmlinkExc> PyEmlinkExc(m, "EmlinkExc" , PyErrnoExc.ptr());
    static py::exception<EpipeExc> PyEpipeExc(m, "EpipeExc" , PyErrnoExc.ptr());
    static py::exception<EdomExc> PyEdomExc(m, "EdomExc" , PyErrnoExc.ptr());
    static py::exception<ErangeExc> PyErangeExc(m, "ErangeExc" , PyErrnoExc.ptr());
    static py::exception<EnomsgExc> PyEnomsgExc(m, "EnomsgExc" , PyErrnoExc.ptr());
    static py::exception<EidrmExc> PyEidrmExc(m, "EidrmExc" , PyErrnoExc.ptr());
    static py::exception<EchrngExc> PyEchrngExc(m, "EchrngExc" , PyErrnoExc.ptr());
    static py::exception<El2nsyncExc> PyEl2nsyncExc(m, "El2nsyncExc" , PyErrnoExc.ptr());
    static py::exception<El3hltExc> PyEl3hltExc(m, "El3hltExc" , PyErrnoExc.ptr());
    static py::exception<El3rstExc> PyEl3rstExc(m, "El3rstExc" , PyErrnoExc.ptr());
    static py::exception<ElnrngExc> PyElnrngExc(m, "ElnrngExc" , PyErrnoExc.ptr());
    static py::exception<EunatchExc> PyEunatchExc(m, "EunatchExc" , PyErrnoExc.ptr());
    static py::exception<EnocsiExc> PyEnocsiExc(m, "EnocsiExc" , PyErrnoExc.ptr());
    static py::exception<El2hltExc> PyEl2hltExc(m, "El2hltExc" , PyErrnoExc.ptr());
    static py::exception<EdeadlkExc> PyEdeadlkExc(m, "EdeadlkExc" , PyErrnoExc.ptr());
    static py::exception<EnolckExc> PyEnolckExc(m, "EnolckExc" , PyErrnoExc.ptr());
    static py::exception<EbadeExc> PyEbadeExc(m, "EbadeExc" , PyErrnoExc.ptr());
    static py::exception<EbadrExc> PyEbadrExc(m, "EbadrExc" , PyErrnoExc.ptr());
    static py::exception<ExfullExc> PyExfullExc(m, "ExfullExc" , PyErrnoExc.ptr());
    static py::exception<EnoanoExc> PyEnoanoExc(m, "EnoanoExc" , PyErrnoExc.ptr());
    static py::exception<EbadrqcExc> PyEbadrqcExc(m, "EbadrqcExc" , PyErrnoExc.ptr());
    static py::exception<EbadsltExc> PyEbadsltExc(m, "EbadsltExc" , PyErrnoExc.ptr());
    static py::exception<EdeadlockExc> PyEdeadlockExc(m, "EdeadlockExc" , PyErrnoExc.ptr());
    static py::exception<EbfontExc> PyEbfontExc(m, "EbfontExc" , PyErrnoExc.ptr());
    static py::exception<EnostrExc> PyEnostrExc(m, "EnostrExc" , PyErrnoExc.ptr());
    static py::exception<EnodataExc> PyEnodataExc(m, "EnodataExc" , PyErrnoExc.ptr());
    static py::exception<EtimeExc> PyEtimeExc(m, "EtimeExc" , PyErrnoExc.ptr());
    static py::exception<EnosrExc> PyEnosrExc(m, "EnosrExc" , PyErrnoExc.ptr());
    static py::exception<EnonetExc> PyEnonetExc(m, "EnonetExc" , PyErrnoExc.ptr());
    static py::exception<EnopkgExc> PyEnopkgExc(m, "EnopkgExc" , PyErrnoExc.ptr());
    static py::exception<EremoteExc> PyEremoteExc(m, "EremoteExc" , PyErrnoExc.ptr());
    static py::exception<EnolinkExc> PyEnolinkExc(m, "EnolinkExc" , PyErrnoExc.ptr());
    static py::exception<EadvExc> PyEadvExc(m, "EadvExc" , PyErrnoExc.ptr());
    static py::exception<EsrmntExc> PyEsrmntExc(m, "EsrmntExc" , PyErrnoExc.ptr());
    static py::exception<EcommExc> PyEcommExc(m, "EcommExc" , PyErrnoExc.ptr());
    static py::exception<EprotoExc> PyEprotoExc(m, "EprotoExc" , PyErrnoExc.ptr());
    static py::exception<EmultihopExc> PyEmultihopExc(m, "EmultihopExc" , PyErrnoExc.ptr());
    static py::exception<EbadmsgExc> PyEbadmsgExc(m, "EbadmsgExc" , PyErrnoExc.ptr());
    static py::exception<EnametoolongExc> PyEnametoolongExc(m, "EnametoolongExc" , PyErrnoExc.ptr());
    static py::exception<EoverflowExc> PyEoverflowExc(m, "EoverflowExc" , PyErrnoExc.ptr());
    static py::exception<EnotuniqExc> PyEnotuniqExc(m, "EnotuniqExc" , PyErrnoExc.ptr());
    static py::exception<EbadfdExc> PyEbadfdExc(m, "EbadfdExc" , PyErrnoExc.ptr());
    static py::exception<EremchgExc> PyEremchgExc(m, "EremchgExc" , PyErrnoExc.ptr());
    static py::exception<ElibaccExc> PyElibaccExc(m, "ElibaccExc" , PyErrnoExc.ptr());
    static py::exception<ElibbadExc> PyElibbadExc(m, "ElibbadExc" , PyErrnoExc.ptr());
    static py::exception<ElibscnExc> PyElibscnExc(m, "ElibscnExc" , PyErrnoExc.ptr());
    static py::exception<ElibmaxExc> PyElibmaxExc(m, "ElibmaxExc" , PyErrnoExc.ptr());
    static py::exception<ElibexecExc> PyElibexecExc(m, "ElibexecExc" , PyErrnoExc.ptr());
    static py::exception<EilseqExc> PyEilseqExc(m, "EilseqExc" , PyErrnoExc.ptr());
    static py::exception<EnosysExc> PyEnosysExc(m, "EnosysExc" , PyErrnoExc.ptr());
    static py::exception<EloopExc> PyEloopExc(m, "EloopExc" , PyErrnoExc.ptr());
    static py::exception<ErestartExc> PyErestartExc(m, "ErestartExc" , PyErrnoExc.ptr());
    static py::exception<EstrpipeExc> PyEstrpipeExc(m, "EstrpipeExc" , PyErrnoExc.ptr());
    static py::exception<EnotemptyExc> PyEnotemptyExc(m, "EnotemptyExc" , PyErrnoExc.ptr());
    static py::exception<EusersExc> PyEusersExc(m, "EusersExc" , PyErrnoExc.ptr());
    static py::exception<EnotsockExc> PyEnotsockExc(m, "EnotsockExc" , PyErrnoExc.ptr());
    static py::exception<EdestaddrreqExc> PyEdestaddrreqExc(m, "EdestaddrreqExc" , PyErrnoExc.ptr());
    static py::exception<EmsgsizeExc> PyEmsgsizeExc(m, "EmsgsizeExc" , PyErrnoExc.ptr());
    static py::exception<EprototypeExc> PyEprototypeExc(m, "EprototypeExc" , PyErrnoExc.ptr());
    static py::exception<EnoprotooptExc> PyEnoprotooptExc(m, "EnoprotooptExc" , PyErrnoExc.ptr());
    static py::exception<EprotonosupportExc> PyEprotonosupportExc(m, "EprotonosupportExc" , PyErrnoExc.ptr());
    static py::exception<EsocktnosupportExc> PyEsocktnosupportExc(m, "EsocktnosupportExc" , PyErrnoExc.ptr());
    static py::exception<EopnotsuppExc> PyEopnotsuppExc(m, "EopnotsuppExc" , PyErrnoExc.ptr());
    static py::exception<EpfnosupportExc> PyEpfnosupportExc(m, "EpfnosupportExc" , PyErrnoExc.ptr());
    static py::exception<EafnosupportExc> PyEafnosupportExc(m, "EafnosupportExc" , PyErrnoExc.ptr());
    static py::exception<EaddrinuseExc> PyEaddrinuseExc(m, "EaddrinuseExc" , PyErrnoExc.ptr());
    static py::exception<EaddrnotavailExc> PyEaddrnotavailExc(m, "EaddrnotavailExc" , PyErrnoExc.ptr());
    static py::exception<EnetdownExc> PyEnetdownExc(m, "EnetdownExc" , PyErrnoExc.ptr());
    static py::exception<EnetunreachExc> PyEnetunreachExc(m, "EnetunreachExc" , PyErrnoExc.ptr());
    static py::exception<EnetresetExc> PyEnetresetExc(m, "EnetresetExc" , PyErrnoExc.ptr());
    static py::exception<EconnabortedExc> PyEconnabortedExc(m, "EconnabortedExc" , PyErrnoExc.ptr());
    static py::exception<EconnresetExc> PyEconnresetExc(m, "EconnresetExc" , PyErrnoExc.ptr());
    static py::exception<EnobufsExc> PyEnobufsExc(m, "EnobufsExc" , PyErrnoExc.ptr());
    static py::exception<EisconnExc> PyEisconnExc(m, "EisconnExc" , PyErrnoExc.ptr());
    static py::exception<EnotconnExc> PyEnotconnExc(m, "EnotconnExc" , PyErrnoExc.ptr());
    static py::exception<EshutdownExc> PyEshutdownExc(m, "EshutdownExc" , PyErrnoExc.ptr());
    static py::exception<EtoomanyrefsExc> PyEtoomanyrefsExc(m, "EtoomanyrefsExc" , PyErrnoExc.ptr());
    static py::exception<EtimedoutExc> PyEtimedoutExc(m, "EtimedoutExc" , PyErrnoExc.ptr());
    static py::exception<EconnrefusedExc> PyEconnrefusedExc(m, "EconnrefusedExc" , PyErrnoExc.ptr());
    static py::exception<EhostdownExc> PyEhostdownExc(m, "EhostdownExc" , PyErrnoExc.ptr());
    static py::exception<EhostunreachExc> PyEhostunreachExc(m, "EhostunreachExc" , PyErrnoExc.ptr());
    static py::exception<EalreadyExc> PyEalreadyExc(m, "EalreadyExc" , PyErrnoExc.ptr());
    static py::exception<EinprogressExc> PyEinprogressExc(m, "EinprogressExc" , PyErrnoExc.ptr());
    static py::exception<EstaleExc> PyEstaleExc(m, "EstaleExc" , PyErrnoExc.ptr());
    static py::exception<EioresidExc> PyEioresidExc(m, "EioresidExc" , PyErrnoExc.ptr());
    static py::exception<EucleanExc> PyEucleanExc(m, "EucleanExc" , PyErrnoExc.ptr());
    static py::exception<EnotnamExc> PyEnotnamExc(m, "EnotnamExc" , PyErrnoExc.ptr());
    static py::exception<EnavailExc> PyEnavailExc(m, "EnavailExc" , PyErrnoExc.ptr());
    static py::exception<EisnamExc> PyEisnamExc(m, "EisnamExc" , PyErrnoExc.ptr());
    static py::exception<EremoteioExc> PyEremoteioExc(m, "EremoteioExc" , PyErrnoExc.ptr());
    static py::exception<EinitExc> PyEinitExc(m, "EinitExc" , PyErrnoExc.ptr());
    static py::exception<EremdevExc> PyEremdevExc(m, "EremdevExc" , PyErrnoExc.ptr());
    static py::exception<EcanceledExc> PyEcanceledExc(m, "EcanceledExc" , PyErrnoExc.ptr());
    static py::exception<EnolimfileExc> PyEnolimfileExc(m, "EnolimfileExc" , PyErrnoExc.ptr());
    static py::exception<EproclimExc> PyEproclimExc(m, "EproclimExc" , PyErrnoExc.ptr());
    static py::exception<EdisjointExc> PyEdisjointExc(m, "EdisjointExc" , PyErrnoExc.ptr());
    static py::exception<EnologinExc> PyEnologinExc(m, "EnologinExc" , PyErrnoExc.ptr());
    static py::exception<EloginlimExc> PyEloginlimExc(m, "EloginlimExc" , PyErrnoExc.ptr());
    static py::exception<EgrouploopExc> PyEgrouploopExc(m, "EgrouploopExc" , PyErrnoExc.ptr());
    static py::exception<EnoattachExc> PyEnoattachExc(m, "EnoattachExc" , PyErrnoExc.ptr());
    static py::exception<EnotsupExc> PyEnotsupExc(m, "EnotsupExc" , PyErrnoExc.ptr());
    static py::exception<EnoattrExc> PyEnoattrExc(m, "EnoattrExc" , PyErrnoExc.ptr());
    static py::exception<EdircorruptedExc> PyEdircorruptedExc(m, "EdircorruptedExc" , PyErrnoExc.ptr());
    static py::exception<EdquotExc> PyEdquotExc(m, "EdquotExc" , PyErrnoExc.ptr());
    static py::exception<EnfsremoteExc> PyEnfsremoteExc(m, "EnfsremoteExc" , PyErrnoExc.ptr());
    static py::exception<EcontrollerExc> PyEcontrollerExc(m, "EcontrollerExc" , PyErrnoExc.ptr());
    static py::exception<EnotcontrollerExc> PyEnotcontrollerExc(m, "EnotcontrollerExc" , PyErrnoExc.ptr());
    static py::exception<EenqueuedExc> PyEenqueuedExc(m, "EenqueuedExc" , PyErrnoExc.ptr());
    static py::exception<EnotenqueuedExc> PyEnotenqueuedExc(m, "EnotenqueuedExc" , PyErrnoExc.ptr());
    static py::exception<EjoinedExc> PyEjoinedExc(m, "EjoinedExc" , PyErrnoExc.ptr());
    static py::exception<EnotjoinedExc> PyEnotjoinedExc(m, "EnotjoinedExc" , PyErrnoExc.ptr());
    static py::exception<EnoprocExc> PyEnoprocExc(m, "EnoprocExc" , PyErrnoExc.ptr());
    static py::exception<EmustrunExc> PyEmustrunExc(m, "EmustrunExc" , PyErrnoExc.ptr());
    static py::exception<EnotstoppedExc> PyEnotstoppedExc(m, "EnotstoppedExc" , PyErrnoExc.ptr());
    static py::exception<EclockcpuExc> PyEclockcpuExc(m, "EclockcpuExc" , PyErrnoExc.ptr());
    static py::exception<EinvalstateExc> PyEinvalstateExc(m, "EinvalstateExc" , PyErrnoExc.ptr());
    static py::exception<EnoexistExc> PyEnoexistExc(m, "EnoexistExc" , PyErrnoExc.ptr());
    static py::exception<EendofminorExc> PyEendofminorExc(m, "EendofminorExc" , PyErrnoExc.ptr());
    static py::exception<EbufsizeExc> PyEbufsizeExc(m, "EbufsizeExc" , PyErrnoExc.ptr());
    static py::exception<EemptyExc> PyEemptyExc(m, "EemptyExc" , PyErrnoExc.ptr());
    static py::exception<EnointrgroupExc> PyEnointrgroupExc(m, "EnointrgroupExc" , PyErrnoExc.ptr());
    static py::exception<EinvalmodeExc> PyEinvalmodeExc(m, "EinvalmodeExc" , PyErrnoExc.ptr());
    static py::exception<EcantextentExc> PyEcantextentExc(m, "EcantextentExc" , PyErrnoExc.ptr());
    static py::exception<EinvaltimeExc> PyEinvaltimeExc(m, "EinvaltimeExc" , PyErrnoExc.ptr());
    static py::exception<EdestroyedExc> PyEdestroyedExc(m, "EdestroyedExc" , PyErrnoExc.ptr());

    py::register_exception_translator([](std::exception_ptr p) {
        // translate c++ exception to python exception
        try {
            if (p) std::rethrow_exception(p);
        }
        //
        //
        catch (const EpermExc &e) {
            PyEpermExc(e.what());
        }
        catch (const EnoentExc &e) {
            PyEnoentExc(e.what());
        }
        catch (const EsrchExc &e) {
            PyEsrchExc(e.what());
        }
        catch (const EintrExc &e) {
            PyEintrExc(e.what());
        }
        catch (const EioExc &e) {
            PyEioExc(e.what());
        }
        catch (const EnxioExc &e) {
            PyEnxioExc(e.what());
        }
        catch (const E2bigExc &e) {
            PyE2bigExc(e.what());
        }
        catch (const EnoexecExc &e) {
            PyEnoexecExc(e.what());
        }
        catch (const EbadfExc &e) {
            PyEbadfExc(e.what());
        }
        catch (const EchildExc &e) {
            PyEchildExc(e.what());
        }
        catch (const EagainExc &e) {
            PyEagainExc(e.what());
        }
        catch (const EnomemExc &e) {
            PyEnomemExc(e.what());
        }
        catch (const EaccesExc &e) {
            PyEaccesExc(e.what());
        }
        catch (const EfaultExc &e) {
            PyEfaultExc(e.what());
        }
        catch (const EnotblkExc &e) {
            PyEnotblkExc(e.what());
        }
        catch (const EbusyExc &e) {
            PyEbusyExc(e.what());
        }
        catch (const EexistExc &e) {
            PyEexistExc(e.what());
        }
        catch (const ExdevExc &e) {
            PyExdevExc(e.what());
        }
        catch (const EnodevExc &e) {
            PyEnodevExc(e.what());
        }
        catch (const EnotdirExc &e) {
            PyEnotdirExc(e.what());
        }
        catch (const EisdirExc &e) {
            PyEisdirExc(e.what());
        }
        catch (const EinvalExc &e) {
            PyEinvalExc(e.what());
        }
        catch (const EnfileExc &e) {
            PyEnfileExc(e.what());
        }
        catch (const EmfileExc &e) {
            PyEmfileExc(e.what());
        }
        catch (const EnottyExc &e) {
            PyEnottyExc(e.what());
        }
        catch (const EtxtbsyExc &e) {
            PyEtxtbsyExc(e.what());
        }
        catch (const EfbigExc &e) {
            PyEfbigExc(e.what());
        }
        catch (const EnospcExc &e) {
            PyEnospcExc(e.what());
        }
        catch (const EspipeExc &e) {
            PyEspipeExc(e.what());
        }
        catch (const ErofsExc &e) {
            PyErofsExc(e.what());
        }
        catch (const EmlinkExc &e) {
            PyEmlinkExc(e.what());
        }
        catch (const EpipeExc &e) {
            PyEpipeExc(e.what());
        }
        catch (const EdomExc &e) {
            PyEdomExc(e.what());
        }
        catch (const ErangeExc &e) {
            PyErangeExc(e.what());
        }
        catch (const EnomsgExc &e) {
            PyEnomsgExc(e.what());
        }
        catch (const EidrmExc &e) {
            PyEidrmExc(e.what());
        }
        catch (const EchrngExc &e) {
            PyEchrngExc(e.what());
        }
        catch (const El2nsyncExc &e) {
            PyEl2nsyncExc(e.what());
        }
        catch (const El3hltExc &e) {
            PyEl3hltExc(e.what());
        }
        catch (const El3rstExc &e) {
            PyEl3rstExc(e.what());
        }
        catch (const ElnrngExc &e) {
            PyElnrngExc(e.what());
        }
        catch (const EunatchExc &e) {
            PyEunatchExc(e.what());
        }
        catch (const EnocsiExc &e) {
            PyEnocsiExc(e.what());
        }
        catch (const El2hltExc &e) {
            PyEl2hltExc(e.what());
        }
        catch (const EdeadlkExc &e) {
            PyEdeadlkExc(e.what());
        }
        catch (const EnolckExc &e) {
            PyEnolckExc(e.what());
        }
        catch (const EbadeExc &e) {
            PyEbadeExc(e.what());
        }
        catch (const EbadrExc &e) {
            PyEbadrExc(e.what());
        }
        catch (const ExfullExc &e) {
            PyExfullExc(e.what());
        }
        catch (const EnoanoExc &e) {
            PyEnoanoExc(e.what());
        }
        catch (const EbadrqcExc &e) {
            PyEbadrqcExc(e.what());
        }
        catch (const EbadsltExc &e) {
            PyEbadsltExc(e.what());
        }
        catch (const EdeadlockExc &e) {
            PyEdeadlockExc(e.what());
        }
        catch (const EbfontExc &e) {
            PyEbfontExc(e.what());
        }
        catch (const EnostrExc &e) {
            PyEnostrExc(e.what());
        }
        catch (const EnodataExc &e) {
            PyEnodataExc(e.what());
        }
        catch (const EtimeExc &e) {
            PyEtimeExc(e.what());
        }
        catch (const EnosrExc &e) {
            PyEnosrExc(e.what());
        }
        catch (const EnonetExc &e) {
            PyEnonetExc(e.what());
        }
        catch (const EnopkgExc &e) {
            PyEnopkgExc(e.what());
        }
        catch (const EremoteExc &e) {
            PyEremoteExc(e.what());
        }
        catch (const EnolinkExc &e) {
            PyEnolinkExc(e.what());
        }
        catch (const EadvExc &e) {
            PyEadvExc(e.what());
        }
        catch (const EsrmntExc &e) {
            PyEsrmntExc(e.what());
        }
        catch (const EcommExc &e) {
            PyEcommExc(e.what());
        }
        catch (const EprotoExc &e) {
            PyEprotoExc(e.what());
        }
        catch (const EmultihopExc &e) {
            PyEmultihopExc(e.what());
        }
        catch (const EbadmsgExc &e) {
            PyEbadmsgExc(e.what());
        }
        catch (const EnametoolongExc &e) {
            PyEnametoolongExc(e.what());
        }
        catch (const EoverflowExc &e) {
            PyEoverflowExc(e.what());
        }
        catch (const EnotuniqExc &e) {
            PyEnotuniqExc(e.what());
        }
        catch (const EbadfdExc &e) {
            PyEbadfdExc(e.what());
        }
        catch (const EremchgExc &e) {
            PyEremchgExc(e.what());
        }
        catch (const ElibaccExc &e) {
            PyElibaccExc(e.what());
        }
        catch (const ElibbadExc &e) {
            PyElibbadExc(e.what());
        }
        catch (const ElibscnExc &e) {
            PyElibscnExc(e.what());
        }
        catch (const ElibmaxExc &e) {
            PyElibmaxExc(e.what());
        }
        catch (const ElibexecExc &e) {
            PyElibexecExc(e.what());
        }
        catch (const EilseqExc &e) {
            PyEilseqExc(e.what());
        }
        catch (const EnosysExc &e) {
            PyEnosysExc(e.what());
        }
        catch (const EloopExc &e) {
            PyEloopExc(e.what());
        }
        catch (const ErestartExc &e) {
            PyErestartExc(e.what());
        }
        catch (const EstrpipeExc &e) {
            PyEstrpipeExc(e.what());
        }
        catch (const EnotemptyExc &e) {
            PyEnotemptyExc(e.what());
        }
        catch (const EusersExc &e) {
            PyEusersExc(e.what());
        }
        catch (const EnotsockExc &e) {
            PyEnotsockExc(e.what());
        }
        catch (const EdestaddrreqExc &e) {
            PyEdestaddrreqExc(e.what());
        }
        catch (const EmsgsizeExc &e) {
            PyEmsgsizeExc(e.what());
        }
        catch (const EprototypeExc &e) {
            PyEprototypeExc(e.what());
        }
        catch (const EnoprotooptExc &e) {
            PyEnoprotooptExc(e.what());
        }
        catch (const EprotonosupportExc &e) {
            PyEprotonosupportExc(e.what());
        }
        catch (const EsocktnosupportExc &e) {
            PyEsocktnosupportExc(e.what());
        }
        catch (const EopnotsuppExc &e) {
            PyEopnotsuppExc(e.what());
        }
        catch (const EpfnosupportExc &e) {
            PyEpfnosupportExc(e.what());
        }
        catch (const EafnosupportExc &e) {
            PyEafnosupportExc(e.what());
        }
        catch (const EaddrinuseExc &e) {
            PyEaddrinuseExc(e.what());
        }
        catch (const EaddrnotavailExc &e) {
            PyEaddrnotavailExc(e.what());
        }
        catch (const EnetdownExc &e) {
            PyEnetdownExc(e.what());
        }
        catch (const EnetunreachExc &e) {
            PyEnetunreachExc(e.what());
        }
        catch (const EnetresetExc &e) {
            PyEnetresetExc(e.what());
        }
        catch (const EconnabortedExc &e) {
            PyEconnabortedExc(e.what());
        }
        catch (const EconnresetExc &e) {
            PyEconnresetExc(e.what());
        }
        catch (const EnobufsExc &e) {
            PyEnobufsExc(e.what());
        }
        catch (const EisconnExc &e) {
            PyEisconnExc(e.what());
        }
        catch (const EnotconnExc &e) {
            PyEnotconnExc(e.what());
        }
        catch (const EshutdownExc &e) {
            PyEshutdownExc(e.what());
        }
        catch (const EtoomanyrefsExc &e) {
            PyEtoomanyrefsExc(e.what());
        }
        catch (const EtimedoutExc &e) {
            PyEtimedoutExc(e.what());
        }
        catch (const EconnrefusedExc &e) {
            PyEconnrefusedExc(e.what());
        }
        catch (const EhostdownExc &e) {
            PyEhostdownExc(e.what());
        }
        catch (const EhostunreachExc &e) {
            PyEhostunreachExc(e.what());
        }
        catch (const EalreadyExc &e) {
            PyEalreadyExc(e.what());
        }
        catch (const EinprogressExc &e) {
            PyEinprogressExc(e.what());
        }
        catch (const EstaleExc &e) {
            PyEstaleExc(e.what());
        }
        catch (const EioresidExc &e) {
            PyEioresidExc(e.what());
        }
        catch (const EucleanExc &e) {
            PyEucleanExc(e.what());
        }
        catch (const EnotnamExc &e) {
            PyEnotnamExc(e.what());
        }
        catch (const EnavailExc &e) {
            PyEnavailExc(e.what());
        }
        catch (const EisnamExc &e) {
            PyEisnamExc(e.what());
        }
        catch (const EremoteioExc &e) {
            PyEremoteioExc(e.what());
        }
        catch (const EinitExc &e) {
            PyEinitExc(e.what());
        }
        catch (const EremdevExc &e) {
            PyEremdevExc(e.what());
        }
        catch (const EcanceledExc &e) {
            PyEcanceledExc(e.what());
        }
        catch (const EnolimfileExc &e) {
            PyEnolimfileExc(e.what());
        }
        catch (const EproclimExc &e) {
            PyEproclimExc(e.what());
        }
        catch (const EdisjointExc &e) {
            PyEdisjointExc(e.what());
        }
        catch (const EnologinExc &e) {
            PyEnologinExc(e.what());
        }
        catch (const EloginlimExc &e) {
            PyEloginlimExc(e.what());
        }
        catch (const EgrouploopExc &e) {
            PyEgrouploopExc(e.what());
        }
        catch (const EnoattachExc &e) {
            PyEnoattachExc(e.what());
        }
        catch (const EnotsupExc &e) {
            PyEnotsupExc(e.what());
        }
        catch (const EnoattrExc &e) {
            PyEnoattrExc(e.what());
        }
        catch (const EdircorruptedExc &e) {
            PyEdircorruptedExc(e.what());
        }
        catch (const EdquotExc &e) {
            PyEdquotExc(e.what());
        }
        catch (const EnfsremoteExc &e) {
            PyEnfsremoteExc(e.what());
        }
        catch (const EcontrollerExc &e) {
            PyEcontrollerExc(e.what());
        }
        catch (const EnotcontrollerExc &e) {
            PyEnotcontrollerExc(e.what());
        }
        catch (const EenqueuedExc &e) {
            PyEenqueuedExc(e.what());
        }
        catch (const EnotenqueuedExc &e) {
            PyEnotenqueuedExc(e.what());
        }
        catch (const EjoinedExc &e) {
            PyEjoinedExc(e.what());
        }
        catch (const EnotjoinedExc &e) {
            PyEnotjoinedExc(e.what());
        }
        catch (const EnoprocExc &e) {
            PyEnoprocExc(e.what());
        }
        catch (const EmustrunExc &e) {
            PyEmustrunExc(e.what());
        }
        catch (const EnotstoppedExc &e) {
            PyEnotstoppedExc(e.what());
        }
        catch (const EclockcpuExc &e) {
            PyEclockcpuExc(e.what());
        }
        catch (const EinvalstateExc &e) {
            PyEinvalstateExc(e.what());
        }
        catch (const EnoexistExc &e) {
            PyEnoexistExc(e.what());
        }
        catch (const EendofminorExc &e) {
            PyEendofminorExc(e.what());
        }
        catch (const EbufsizeExc &e) {
            PyEbufsizeExc(e.what());
        }
        catch (const EemptyExc &e) {
            PyEemptyExc(e.what());
        }
        catch (const EnointrgroupExc &e) {
            PyEnointrgroupExc(e.what());
        }
        catch (const EinvalmodeExc &e) {
            PyEinvalmodeExc(e.what());
        }
        catch (const EcantextentExc &e) {
            PyEcantextentExc(e.what());
        }
        catch (const EinvaltimeExc &e) {
            PyEinvaltimeExc(e.what());
        }
        catch (const EdestroyedExc &e) {
            PyEdestroyedExc(e.what());
        }
        //
        catch (const ArgExc &e) {
            PyArgExc(e.what());
        }
        catch (const LogicExc &e) {
            PyLogicExc(e.what());
        }
        catch (const InputExc &e) {
            PyInputExc(e.what());
        }
        catch (const IoExc &e) {
            PyIoExc(e.what());
        }
        catch (const MathExc &e) {
            PyMathExc(e.what());
        }
        catch (const NoImplExc &e) {
            PyNoImplExc(e.what());
        }
        catch (const NullExc &e) {
            PyNullExc(e.what());
        }
        catch (const TypeExc &e) {
            PyTypeExc(e.what());
        }
        catch (const ErrnoExc &e) {
            PyErrnoExc(e.what());
        }
        //
        catch (const BaseExc &e) {
            PyBaseExc(e.what());
        }
        catch (const std::exception &e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }
    });
}
