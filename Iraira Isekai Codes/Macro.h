#pragma once

// 사용자 정의 문자열 관련 자료형 및 함수
#ifdef UNICODE
#define tstring wstring
#define _tfopen_s(file, filename, mode) _wfopen_s(file, filename, mode)
#else
#define tstring string
#endif

// Delete, Release 관련 Safe 매크로 함수
#define SAFE_DELETE(p)			if (p) { delete p; p = NULL; }
#define SAFE_RELEASE(p)			if (p) { p->Release(); p = NULL; }
#define SAFE_ARRAY_DELETE(p)	if (p) { delete[] p; p = NULL; }
#define SAFE_ARRAY_RELEASE(T, p)	\
	for (size_t i = 0; i < sizeof(p) / sizeof(T); i++)	\
		p[i]->Release();
template <typename T>
void Safe_Veclist_Delete(T &p) {
	typename T::iterator it;
	typename T::iterator it_end = p.end();
	for (it = p.begin(); it != it_end; it++)
		SAFE_DELETE((*it));
	p.clear();
}
template <typename T>
static void Safe_Static_Veclist_Delete(T &p) {
	typename T::iterator it;
	typename T::iterator it_end = p.end();
	for (it = p.begin(); it != it_end; it++)
		SAFE_DELETE((*it));
	p.clear();
}
template <typename T>
void Safe_Map_Delete(T &p) {
	typename T::iterator it;
	typename T::iterator it_end = p.end();
	for (it = p.begin(); it != it_end; it++)
		SAFE_DELETE(it->second);
	p.clear();
}
template <typename T>
static void Safe_Static_Map_Delete(T &p) {
	typename T::iterator it;
	typename T::iterator it_end = p.end();
	for (it = p.begin(); it != it_end; it++)
		SAFE_DELETE(it->second);
	p.clear();
}
template <typename T>
void Safe_Veclist_Release(T &p) {
	typename T::iterator it;
	typename T::iterator it_end = p.end();
	for (it = p.begin(); it != it_end; it++)
		SAFE_RELEASE((*it));
	p.clear();
}
template <typename T>
static void Safe_Static_Veclist_Release(T &p) {
	typename T::iterator it;
	typename T::iterator it_end = p.end();
	for (it = p.begin(); it != it_end; it++)
		SAFE_RELEASE((*it));
	p.clear();
}
template <typename T>
void Safe_Map_Release(T &p) {
	typename T::iterator it;
	typename T::iterator it_end = p.end();
	for (it = p.begin(); it != it_end; it++)
		SAFE_RELEASE(it->second);
	p.clear();
}
template <typename T>
static void Safe_Static_Map_Release(T &p) {
	typename T::iterator it;
	typename T::iterator it_end = p.end();
	for (it = p.begin(); it != it_end; it++)
		SAFE_RELEASE(it->second);
	p.clear();
}
#define SAFE_VECLIST_DELETE(p)			Safe_Veclist_Delete(p)
#define SAFE_STATIC_VECLIST_DELETE(p)	Safe_Static_Veclist_Delete(p)
#define SAFE_MAP_DELETE(p)				Safe_Map_Delete(p)
#define SAFE_STATIC_MAP_DELETE(p)		Safe_Static_Map_Delete(p)
#define SAFE_VECLIST_RELEASE(p)			Safe_Veclist_Release(p)
#define SAFE_STATIC_VECLIST_RELEASE(p)	Safe_Static_Veclist_Release(p)
#define SAFE_MAP_RELEASE(p)				Safe_Map_Release(p)
#define SAFE_STATIC_MAP_RELEASE(p)		Safe_Static_Map_Release(p)

// 싱글턴 관련 매크로 함수
#define DECLARE_SINGLE(Type)			\
	private:							\
		static Type* m_pInst;			\
		Type();							\
		~Type();						\
	public:								\
		static Type* Get_Inst() {		\
			if (!m_pInst)				\
				m_pInst = new Type;		\
			return m_pInst;				\
		}								\
		static void Destroy_Inst() {	\
			SAFE_DELETE(m_pInst);		\
		}								\
		static bool Single_Declared() {	\
			if (m_pInst)				\
				return TRUE;			\
			else						\
				return FALSE;			\
		}
#define DEFINITION_SINGLE(Type)	Type* Type::m_pInst = NULL
#define GET_SINGLE(Type)		Type::Get_Inst()
#define SINGLE_DECLARED(Type)	Type::Single_Declared()
#define DESTROY_SINGLE(Type)	Type::Destroy_Inst()