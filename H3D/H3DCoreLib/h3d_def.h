/********************************************************************
	created:	2012/02/29
	created:	29:2:2012   1:41
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_def.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_def_h__
#define h3d_def_h__
//   类
namespace Heaven3D
{

#define H3D_SAFE_DELETE(p)			do{ if(p) { delete (p);     (p)=NULL; } }while(0)
#define H3D_SAFE_DELETE_ARRAY(p)	do{ if(p) { delete[] (p);   (p)=NULL; } }while(0)
#define H3D_SAFE_RELEASE(p)			do{ if(p) { (p)->Release(); (p)=NULL; } }while(0)
#define H3D_IF_FALSE_RET_FALSE(p)	do{ if(!(p)) {return false;} }while(0)
#define H3D_IF_FAILED_RET_FALSE(r)	do{ if(FAILED((r))){return false;}}while(0)
#define H3D_INLINE __forceinline

}

#endif // h3d_def_h__