/* 
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *  
 *  Copyright (C) 2011-2012 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2011-2012 by FUJISOFT INCORPORATED, JAPAN
 *  Copyright (C) 2011-2012 by FUJITSU VLSI LIMITED, JAPAN
 *  Copyright (C) 2011-2012 by NEC Communication Systems, Ltd., JAPAN
 *  Copyright (C) 2011-2012 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 *  Copyright (C) 2011-2012 by Renesas Electronics Corporation, JAPAN
 *  Copyright (C) 2011-2012 by Sunny Giken Inc., JAPAN
 *  Copyright (C) 2011-2012 by TOSHIBA CORPOTATION, JAPAN
 *  Copyright (C) 2011-2012 by Witz Corporation, JAPAN
 *  Copyright (C) 2012 by TAKAGI Nobuhisa
 *  
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *  
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: xml_parser.cpp 5785 2012-09-25 06:08:21Z mtakada $
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include "toppers/diagnostics.hpp"
#include "xml_object.hpp"

using namespace std;
using namespace toppers;
using namespace toppers::xml::container;

// ---------------------------------------------------------------------------
//  SAX2Handlers: Constructors and Destructor
// ---------------------------------------------------------------------------
SAX2Handlers::SAX2Handlers() :

fEcuModuleConfigurationValues_(0)
, fEcucContainerValue_(0)
, fParameterValues_(0)
, fReferenceValues_(0)
, fSubcontainers_(0)
, fSubcontainers_old_(0)
, fAttrXmlSpace_(true)
/*
, fEcucReferenceValue_(0)
, fEcucNumericalParamValue_(0)
, fEcucTextualParamValue_(0)
*/
{
}

SAX2Handlers::~SAX2Handlers()
{
}

// ---------------------------------------------------------------------------
//  SAX2CountHandlers: Implementation of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void SAX2Handlers::startElement(const XMLCh* const uri
                   , const XMLCh* const localname
                   , const XMLCh* const qname
                   , const Attributes& attrs)
{
  static XercesString ecucmodule    = fromNative("ECUC-MODULE-CONFIGURATION-VALUES");
  static XercesString ecuccontainer = fromNative("ECUC-CONTAINER-VALUE");
  static XercesString subcontainer  = fromNative("SUB-CONTAINERS");
  static XercesString parameter     = fromNative("PARAMETER-VALUES");
  static XercesString reference     = fromNative("REFERENCE-VALUES");
  static XercesString ecuctextual   = fromNative("ECUC-TEXTUAL-PARAM-VALUE");
  static XercesString ecucnumerical = fromNative("ECUC-NUMERICAL-PARAM-VALUE");
  static XercesString ecucreference = fromNative("ECUC-REFERENCE-VALUE");
  static XercesString definitionref = fromNative("DEFINITION-REF");
  static XercesString defqname      = fromNative("DEST");
  static XercesString defInteger    = fromNative("ECUC-INTEGER-PARAM-DEF");
  static XercesString defFloat      = fromNative("ECUC-FLOAT-PARAM-DEF");
  static XercesString defString     = fromNative("ECUC-STRING-PARAM-DEF");
  static XercesString defBool       = fromNative("ECUC-BOOLEAN-PARAM-DEF");
  static XercesString defEnum       = fromNative("ECUC-ENUMERATION-PARAM-DEF");
  static XercesString defReference  = fromNative("ECUC-REFERENCE-DEF");
  static XercesString defFunction   = fromNative("ECUC-FUNCTION-NAME-DEF");

//  XERCES_STD_QUALIFIER cerr << "element : " << toNative(localname) << "(" << get_line() << ")" << XERCES_STD_QUALIFIER endl;
  int attlen = attrs.getLength();
  for(int i=0 ; i < attlen ; i++)
  {
    const XMLCh* qname = attrs.getQName(i);
    string name = toNative(attrs.getValue(qname));
    //XERCES_STD_QUALIFIER cerr << "        ATTR("<< i <<":"<< attlen <<") : name : " << toNative(qname) << "[" << name << "]" << XERCES_STD_QUALIFIER endl;
    if( toNative(qname) == "xml:space" && name == "preserve" )
    {
      fAttrXmlSpace_ = false;
    }
  }

  if(localname == ecucmodule) {
    fEcuModuleConfigurationValues_++;

    obj_temp = new toppers::xml::container::object();
    object_array.push_back(obj_temp);
    obj_temp->setParent( obj_temp );
    obj_temp->setLine( get_line() );
    obj_temp->setId( 1 );

  } else if(localname == ecuccontainer) {
    fEcucContainerValue_++;

    toppers::xml::container::object *old_obj = obj_temp;
    obj_temp = new toppers::xml::container::object();
    obj_temp->setLine( get_line() );

    if(fSubcontainers_ > fSubcontainers_old_) // old_objのサブコンテナの場合
    {
      old_obj->getSubcontainers()->push_back(obj_temp);
      obj_temp->setParent( old_obj );
    }
    else
    {
      while(fSubcontainers_ < fSubcontainers_old_)
      {
        old_obj = old_obj->getParent();
        fSubcontainers_old_--;
      }
      old_obj->getParent()->getSubcontainers()->push_back(obj_temp);
      obj_temp->setParent( old_obj->getParent() );
    }

    fSubcontainers_old_ = fSubcontainers_;
  }

  // コンテナ内のタグ情報をパースする
  if(fEcuModuleConfigurationValues_ || fEcucContainerValue_)
  {
    
    if(localname == subcontainer)
    {
      fSubcontainers_++;
    }
    else if(localname == parameter)
    {
      fParameterValues_++;
    }
    else if(localname == reference)
    {
      fReferenceValues_++;
    }
    else if(localname == ecuctextual || localname == ecucnumerical || localname == ecucreference)
    {
      para_temp = new toppers::xml::container::parameter();
      para_temp->setLine( get_line() );
      para_temp->setFileName( filename );
      para_temp->setParent( obj_temp );
      obj_temp->getParams()->push_back(para_temp);

      if(localname == ecuctextual)
      {
        //fEcucTextualParamValue_++;
        para_temp->setType( TYPE_STRING );
      }
      else if(localname == ecucnumerical)
      {
        //fEcucNumericalParamValue_++;
        para_temp->setType( TYPE_INT );
      }
      else if (localname == ecucreference)
      {
        //fEcucReferenceValue_++;
        para_temp->setType( TYPE_REF );
      }
    }
    else if(localname == definitionref)
    {
      int attlen = attrs.getLength();
      for(int i=0 ; i < attlen ; i++)
      {
        const XMLCh* qname = attrs.getQName(i);
        const XMLCh* value = attrs.getValue(qname);
#if defined( _MSC_VER ) && _DEBUG && XML_DEBUG
        XERCES_STD_QUALIFIER cerr << "        ATTR : name : " << toNative(qname) << "[" << toNative(value) << "]" << XERCES_STD_QUALIFIER endl;
#endif
        if(defqname== qname && para_temp != NULL)
        {
          if(value == defInteger)
          {
            para_temp->setType( TYPE_INT );
          }
          else if(value == defFloat)
          {
            para_temp->setType( TYPE_FLOAT );
          }
          else if(value == defString)
          {
            para_temp->setType( TYPE_STRING );
          }
          else if(value == defBool)
          {
            para_temp->setType( TYPE_BOOLEAN );
          }
          else if(value == defEnum)
          {
            para_temp->setType( TYPE_ENUM );
          }
          else if(value == defReference)
          {
            para_temp->setType( TYPE_REF );
          }
          else if(value == defFunction)
          {
            para_temp->setType( TYPE_FUNCTION );
          }
        }
      }
    }
  }
}

void SAX2Handlers::characters(  const   XMLCh* const   chars
                   , const XMLSize_t length)
{
  string slen = boost::lexical_cast<string>(length);
#if defined( _MSC_VER ) && _DEBUG && XML_DEBUG
  XERCES_STD_QUALIFIER cerr << "contents : " << toNative(chars) << "(" << get_line() << ")" << XERCES_STD_QUALIFIER endl;
#endif

  if(fEcuModuleConfigurationValues_)
  {
    currentText_.append(chars, length);
  }
}

void SAX2Handlers::endElement( const XMLCh* const uri, const XMLCh *const localname, const XMLCh *const qname)
{

#if defined( _MSC_VER ) && _DEBUG && XML_DEBUG
  XERCES_STD_QUALIFIER cerr << "end element : " << toNative(localname) << XERCES_STD_QUALIFIER endl;
#endif

  static XercesString ecucmodule    = fromNative("ECUC-MODULE-CONFIGURATION-VALUES");
  static XercesString ecuccontainer = fromNative("ECUC-CONTAINER-VALUE");
  static XercesString subcontainer  = fromNative("SUB-CONTAINERS");
  static XercesString parameter     = fromNative("PARAMETER-VALUES");
  static XercesString reference     = fromNative("REFERENCE-VALUES");
  static XercesString ecuctextual   = fromNative("ECUC-TEXTUAL-PARAM-VALUE");
  static XercesString ecucnumerical = fromNative("ECUC-NUMERICAL-PARAM-VALUE");
  static XercesString ecucreference = fromNative("ECUC-REFERENCE-VALUE");

  static XercesString definitionref = fromNative("DEFINITION-REF");
  static XercesString valueref      = fromNative("VALUE-REF");
  static XercesString shortname     = fromNative("SHORT-NAME");
  static XercesString value         = fromNative("VALUE");

  static XercesString oscontainer   = fromNative("/AUTOSAR/EcucDefs/Os");

  // パース中のタグの処理
  if(fEcuModuleConfigurationValues_ || fEcucContainerValue_)
  {
    // xml:space要素はデフォルトでスペースを削除する
    if(fAttrXmlSpace_ == true)
    {
      //XERCES_STD_QUALIFIER cerr << "attr(before) : [" << toNative(currentText_) << "]" <<XERCES_STD_QUALIFIER endl;
      std::string strAttr = ( toNative(currentText_) );
      boost::trim(strAttr);
      currentText_ = fromNative( strAttr.c_str() );
      //XERCES_STD_QUALIFIER cerr << "attr(after) : [" << toNative(currentText_) << "]" <<XERCES_STD_QUALIFIER endl;
    }
    if(localname == shortname)
    {
      obj_temp->setObjName( toNative(currentText_) );
      obj_temp->setFileName( filename );
    }
    else if(localname == definitionref)
    {
      if(fParameterValues_ || fReferenceValues_)
      {
        para_temp->setDefName( toNative(currentText_) );
      }
      else
      {
        obj_temp->setDefName( toNative(currentText_) );
      }
    }
    else if(localname == value || localname == valueref)
    {
      para_temp->setValue( toNative(currentText_) );
    }


    else if(localname == ecucmodule)
    {
      fEcuModuleConfigurationValues_--;
    }
    else if(localname == ecuccontainer)
    {
      fEcucContainerValue_--;
    }
    else if(localname == subcontainer)
    {
      fSubcontainers_--;
    }
    else if (localname == parameter)
    {
      fParameterValues_--;
    }
    else if (localname == reference)
    {
      fReferenceValues_--;
    }
    /* 以下の処理はフラグの比較を使わないのであれば要らないと思われる */
/*    else if (localname == ecuctextual)
    {
      fEcucTextualParamValue_--;
    }
    else if (localname == ecucnumerical)
    {
      fEcucNumericalParamValue_--;
    }
    else if (localname == ecucreference)
    {
      fEcucReferenceValue_--;
    }
*/
  }
  currentText_.clear();
  fAttrXmlSpace_ = true;
}

void SAX2Handlers::setDocumentLocator (const Locator *const locator)
{
  locator_ = locator;
}

int SAX2Handlers::get_line()
{
  return (int)locator_->getLineNumber();
}

// 要素コンテンツで無視できる空白や改行処理
void SAX2Handlers::ignorableWhitespace( const   XMLCh* const chars
                      , const XMLSize_t length)
{
}

/*
void SAX2Handlers::startDocument()
{
}

void SAX2Handlers::endDocument()
{
}
*/

// object_arrayの回収
void SAX2Handlers::obj_delete(toppers::xml::container::object *pObj)
{
  while( !pObj->getSubcontainers()->empty() )
  {
    obj_delete( pObj->getSubcontainers()->back() );
    delete pObj->getSubcontainers()->back();
    pObj->getSubcontainers()->pop_back();
  }
  
  for(std::vector<toppers::xml::container::parameter*>::iterator pPara = pObj->getParams()->begin() ;
    pPara != pObj->getParams()->end(); ++pPara)
  {
    delete *pPara;
  }
  pObj->getParams()->clear();
}

// ---------------------------------------------------------------------------
//  SAX2CountHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void SAX2Handlers::error(const SAXParseException& e)
{
   cerr << "\nError at file " << toNative(e.getSystemId())
    << ", line " << e.getLineNumber()
    << ", char " << e.getColumnNumber()
    << "\n  Message: " << toNative(e.getMessage()) << endl;

}

void SAX2Handlers::fatalError(const SAXParseException& e)
{
   cerr << "\nFatal Error at file " << toNative(e.getSystemId())
    << ", line " << e.getLineNumber()
    << ", char " << e.getColumnNumber()
    << "\n  Message: " << toNative(e.getMessage()) << endl;

}

void SAX2Handlers::warning(const SAXParseException& e)
{
  cerr << "\nWarning at file " << toNative(e.getSystemId())
    << ", line " << e.getLineNumber()
    << ", char " << e.getColumnNumber()
    << "\n  Message: " << toNative(e.getMessage()) << endl;
}

void SAX2Handlers::resetErrors()
{
}
