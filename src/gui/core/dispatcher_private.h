#pragma once

#include"common\common.h"
#include"core\inputEvent.h"
#include"core\debug.h"
#include"utils\typeSet.h"

#include<type_traits>

namespace gui
{
	struct Dispatcher_implementation
	{
		/**
		*	\brief ����ʹ�ú���ʵ�����е��¼��ź�����SignalType
		*/
#define IMPLEMENT_EVENT_SIGNA(SET,FUNCTION,QUEUE)						\
		/**																\
		*	\brief ��ȡ��Ӧdispatcher��SignalType						\
		*	\param K	 �¼���											\
		*	\param event �����¼�										\
		*/																\
		template<typename K>											\
		static std::enable_if_t<util::typeset_exist<SET, K>::value, Dispatcher::SignalType<FUNCTION> >& \
			EventSinal(Dispatcher& dispatcher, const ui_event event)	\
		{																\
			return dispatcher.QUEUE.mQueue[event];						\
		}																\
		/**																\
		*	\brief ��ȡ��Ӧdispatcher��SignalType						\
		*	\param F �źź���											\
		*	\param event �����¼�										\
		*/																\
		template<typename F>											\
		static std::enable_if_t<std::is_same<F, FUNCTION>::value, Dispatcher::SignalType<FUNCTION> >& \
		EventSinal(Dispatcher& dispatcher, const ui_event event)		\
		{																\
			return dispatcher.QUEUE.mQueue[event];						\
		}	

		IMPLEMENT_EVENT_SIGNA(setEvent, SignalFunction, mQueueSignal)

#define IMPLEMENT_EVENT_SIGNAL_WRAPPER(TYPE)		 \
			IMPLEMENT_EVENT_SIGNA(setEvent##TYPE,	   \
								  SignalFunction##TYPE, \
								  mQueueSignal##TYPE)

			IMPLEMENT_EVENT_SIGNAL_WRAPPER(Mouse)
			IMPLEMENT_EVENT_SIGNAL_WRAPPER(Keyboard)

#undef IMPLEMENT_EVENT_SIGNAL_WRAPPER
#undef IMPLEMENT_EVENT_SIGNA

		/**
		*	\brief HasHandler��α������ʽ���ڰ������һ��dispahcer
		*	�Ƿ��ܹ�����һ���¼�������singal_queue���ź����Ƿ�Ϊ��
		*/
		class HasHandler
		{
		public:
			HasHandler(const Dispatcher::event_queue_type type, Dispatcher& dispatcher) :
				mEventQueueType(type),
				mDispatcher(dispatcher)
			{
			}

			/**
			*	\brief ����Ƿ�����¼�
			*	\param T �źź���
			*	\param event ������¼�
			*/
			template<typename T>
			bool operator()(ui_event event)
			{
				if ((mEventQueueType & Dispatcher::pre) &&
					!EventSinal<T>(mDispatcher,event).mPreChild.empty())
				{
					return true;
				}

				if ((mEventQueueType & Dispatcher::child) &&
					!EventSinal<T>(mDispatcher, event).mChild.empty())
				{
					return true;
				}

				if ((mEventQueueType & Dispatcher::post) &&
					!EventSinal<T>(mDispatcher, event).mPostChild.empty())
				{
					return true;
				}

				return false;
			}

		private:
			Dispatcher::event_queue_type mEventQueueType;
			Dispatcher& mDispatcher;
		};
	};


	/**
	*	\brief ģ��������ʵ��
	*/
	namespace implementation
	{
		// �ݹ��ս�
		template<bool done = true>
		struct Find
		{
			template< class E, class F>
			static bool execute(const ui_event event, F function)
			{
				return false;
			}
		};

		// ѭ������typeset
		template<>
		struct Find<false>
		{
			template< class E, class F>
			static bool execute(const ui_event event, F function)
			{
				// ����


				return implementation::Find<std::is_same<begin, end>::value>::execute(
					event, func);
			}
		};
	}

	/**
	*	\brief ͨ�õ��¼����Һ���������ΪF�ĺ���
	*/
	template<typename EventSet, class E, class F>
	bool Find(E event, F func)
	{
		using begin = util::typeset_begin<EventSet>::type;
		using end   = util::typeset_end<EventSet>::type;

		if (begin::value == event)
		{
			return func.operator()<begin>(event);
		}
		else
		{
			return implementation::Find<std::is_same<begin, end>::value>::execute(
				event, func);
		}
	}

	namespace implementation
	{
		/**
		*	\brief ��ȡһ����ǰwidge�¼�������
		*	\param event ���ݵ�event
		*	\param dispathcer ���ϲ��dispathcer
		*	\param w �¼��Ľ�����
		*
		*	�¼����Ӹ�widget��ʼ�����ഫ��ֱ��dispatcherֹͣ
		*/
		template<typename T>
		inline std::vector<std::pair<Widget*, ui_event> > 
			BuildEventChain(const ui_event event, Widget*dispatcher, Widget* w)
		{
			Debug::CheckAssertion(dispatcher != nullptr);
			Debug::CheckAssertion(w != nullptr);

			std::vector<std::pair<Widget*, ui_event> > eventChain;

			while (true)
			{
				if (w->HasEvent(event, Dispatcher::event_queue_type(Dispatcher::pre | Dispatcher::post)) )
				{
					eventChain.emplace_back(w, event);
				}

				if (w == dispatcher)
				{
					break;
				}

				w = w->GetParent();
				if (w == nullptr)
				{
					break;
				}
			}

			return eventChain;
		}

		template<typename T, typename... Args>
		inline bool FireEvent(const ui_event event, std::vector<std::pair<Widget*, ui_event> >& eventChain, Dispatcher*dispatcher, Widget* w, F&&... params)
		{
		}
	}

	template<typename T, typename... F>
	inline bool FireEvent(const ui_event event, Dispatcher*dispatcher, Widget* w, F&&... params)
	{
		return true;
	}

}