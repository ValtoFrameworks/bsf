#pragma once

#include "BsEditorPrerequisites.h"
#include "BsEditorWidgetManager.h"
#include "boost/signal.hpp"

namespace BansheeEditor
{
	class BS_ED_EXPORT EditorWidgetBase
	{
	public:
		const CM::String& getName() const { return mName; }
		const CM::HString& getDisplayName() const { return mDisplayName; }

		CM::INT32 getX() const { return mX; }
		CM::INT32 getY() const { return mY; }
		CM::UINT32 getWidth() const { return mWidth; }
		CM::UINT32 getHeight() const { return mHeight; }

		void _setSize(CM::UINT32 width, CM::UINT32 height);
		void _setPosition(CM::INT32 x, CM::INT32 y);
		void _changeParent(EditorWidgetContainer* parent);
		EditorWidgetContainer* _getParent() const { return mParent; }

		void _disable();
		void _enable();

		void close();

		boost::signal<void(CM::UINT32, CM::UINT32)> onResized;
		boost::signal<void(CM::INT32, CM::INT32)> onMoved;
		boost::signal<void(EditorWidgetContainer*)> onParentChanged;
	protected:
		friend class EditorWidgetManager;

		EditorWidgetBase(const CM::HString& displayName, const CM::String& name, EditorWidgetContainer& parentContainer);
		virtual ~EditorWidgetBase();

		CM::String mName;
		CM::HString mDisplayName;
		EditorWidgetContainer* mParent;
		CM::INT32 mX, mY;
		CM::UINT32 mWidth, mHeight;
		BS::GUIArea* mContent;

		BS::GUIWidget& getParentWidget() const;

		static void destroy(EditorWidgetBase* widget);
	};

	template<typename Type>
	struct RegisterWidgetOnStart
	{
	public:
		RegisterWidgetOnStart()
		{
			EditorWidgetManager::preRegisterWidget(Type::getTypeName(), &create);
		}

		static EditorWidgetBase* create(EditorWidgetContainer& parentContainer)
		{
			return cm_new<Type>(EditorWidget<Type>::ConstructPrivately(), parentContainer);
		}

		void makeSureIAmInstantiated() { }
	};

	template <class Type>
	class EditorWidget : public EditorWidgetBase
	{
		static RegisterWidgetOnStart<Type> RegisterOnStart;

	protected:
		friend struct RegisterWidgetOnStart<Type>;

		struct ConstructPrivately {};

		EditorWidget(const CM::HString& displayName, EditorWidgetContainer& parentContainer)
			:EditorWidgetBase(displayName, Type::getTypeName(), parentContainer)
		{
			RegisterOnStart.makeSureIAmInstantiated();
		}

	public:

		virtual ~EditorWidget() { }
	};

	template <typename Type>
	RegisterWidgetOnStart<Type> EditorWidget<Type>::RegisterOnStart;
}