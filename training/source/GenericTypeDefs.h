/*
 * GenericTypeDefs.h
 *
 *  Created on: Jul 1, 2011
 *      Author: edielson
 */

#ifndef GENERICTYPEDEFS_H_
#define GENERICTYPEDEFS_H_

typedef unsigned char		Bool8;	/* b */
typedef unsigned char           uInt8;	/* uc */
typedef unsigned short int      uInt16;	/* ui */
typedef unsigned long int       uInt32;	/* ul */
typedef unsigned long long int  uInt64;	/* ull */
typedef char			Int8;	/* c */
typedef short int		Int16;	/* i */
typedef long int		Int32;	/* l */
typedef long long int		Int64;	/* ll */
typedef unsigned char*		puInt8;	/* c */
typedef char*			pInt8;	/* c */

#ifndef TRUE8
#define TRUE8	(Bool8) 1
#endif
#ifndef FALSE8
#define FALSE8	(Bool8) 0
#endif

#endif /* GENERICTYPEDEFS_H_ */
