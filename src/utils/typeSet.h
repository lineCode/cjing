#pragma once

#include"common\common.h"
#include<type_traits>

namespace util
{
/**
*	\brief ���ͼ��Ͻṹ
*
*	���ͼ��Ͻṹ�ṩһ�ֱ������ļ��Ͻṹ�������������б�
*	�ṩ��ɾ��ӿڣ������ڱ������ػ�ʹ�õĺ���
*/
template<typename ...>
struct  typeset
{
};
/******  typeSet method ******/

/*** type check **/
template<typename T>
struct isTypeset : std::false_type
{};

template<typename... T>
struct isTypeset<typeset<T...>> : std::true_type
{};

/**
*	\brief ����Ƿ���typeset
*/
template<typename Set>
struct checkTypeset
{
	static_assert(isTypeset<Set>::value, "The template param is not typeset.");
};

/*********** size  **********/
template<typename T>
struct typeset_size : std::integral_constant<int, 0>, checkTypeset<T>
{};
template<typename... T>
struct typeset_size<typeset<T...>> : std::integral_constant<int, sizeof...(T)>
{};

template<typename T, int index>
struct check_index_vaild
{
	static_assert( index >= 0,					   "index is invaild.");
	static_assert( index < typeset_size<T>::value, "index is invaild.");
};

// note: ʵ��������ģ������Ĵ��ݺ�"�ػ�"ԭ�����ݴ���
//		 ��ģ���������ȥ�����ػ���ģ��ṹ��
template<typename T>
struct typeset_empty : std::true_type, checkTypeset<T>
{};
template<typename... T>
struct typeset_empty<typeset<T...>> : std::false_type
{};
template<>
struct  typeset_empty<typeset<>> : std::true_type
{};

/**********  visit ********/
template<typename T, int index>
struct typeset_at : check_index_vaild<T, index>
{
	using type = T;
};
template<typename T1, typename... T, int N>
struct typeset_at<typeset<T1,T...>,N> : typeset_at<typeset<T...>,N-1>
{};
template<typename T1, typename... T>
struct typeset_at<typeset<T1, T...>,0>
{
	using type = T1;
};

template<typename T>
using typeset_begin = typeset_at<T, 0>;

template<typename T>
using typeset_end  = typeset_at<T, typeset_size<T>::value - 1>;

/**********  find ************/
// û�ҵ��ػ�
template<typename Typeset, typename T>
struct typeset_find : std::integral_constant<int, -1>,
					checkTypeset<Typeset>
{};
// �ҵ����ػ�
template<typename T1, typename... T>					// ��ǰ��type�е�һ����Ŀ��type��ȣ�T1��T1
struct typeset_find<typeset<T1, T...>, T1> : std::integral_constant<int, 0>
{};

template<typename T1, typename... T, typename U>		// ��ǰ��type�е�һ����Ŀ��type����ȣ�T1��U
struct typeset_find<typeset<T1, T...>, U> : 
		 std::integral_constant<int, 
		 (typeset_find<typeset<T...>,U>::value == -1 ? -1 :
		 typeset_find<typeset<T...>, U>::value + 1)>   // �ҵ���һ��Ҳ�Ǵ�0��ʼ����Ҫ+1
{};

template<typename Typeset, typename T>
struct typeset_exist : std::integral_constant<bool, (typeset_find<Typeset, T>::value != -1)>
{};

template<typename Typeset,typename T>
using typeset_next = typeset_at<Typeset, typeset_find<Typeset, T>::value + 1>;



}


//enum test_event
//{
//	event_1,
//	event_2,
//	event_3
//};
//template<typename T, test_event V>
//using int_ = std::integral_constant<T, V>;

//using MouseEventSet = util::typeset<int_<int, event_1>,
//	int_<int, event_2>,
//	int_<int, event_3 >> ;
//util::checkTypeset<MouseEventSet>();
//auto length = util::typeset_size<MouseEventSet>::value;
//bool empty = util::typeset_empty<MouseEventSet>::value;
//using newType = util::typeset_end<MouseEventSet>::type;
//auto value = newType::value;