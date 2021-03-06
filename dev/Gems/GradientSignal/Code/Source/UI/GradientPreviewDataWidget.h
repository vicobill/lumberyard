/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzToolsFramework/UI/PropertyEditor/PropertyEditorAPI.h>
#include <GradientSignal/Ebuses/GradientPreviewRequestBus.h>
#include <GradientSignal/GradientSampler.h>
#include <UI/GradientPreviewWidget.h>
#include <LmbrCentral/Dependency/DependencyMonitor.h>
#include <LmbrCentral/Dependency/DependencyNotificationBus.h>

#include <QWidget>

namespace GradientSignal
{
    class GradientPreviewWidget;

    class GradientPreviewDataWidget
        : public QWidget
        , public LmbrCentral::DependencyNotificationBus::Handler
        , public GradientPreviewRequestBus::Handler
    {
    public:
        GradientPreviewDataWidget(QWidget* parent = nullptr);
        ~GradientPreviewDataWidget() override;

        void SetGradientSampler(const GradientSampler& sampler);
        void SetGradientSampleFilter(GradientPreviewWidget::SampleFilterFunc sampleFunc);
        void SetGradientEntity(const AZ::EntityId& id);

        //////////////////////////////////////////////////////////////////////////
        // LmbrCentral::DependencyNotificationBus::Handler
        void OnCompositionChanged() override;

        //////////////////////////////////////////////////////////////////////////
        // GradientPreviewRequestBus::Handler
        void Refresh() override;
        void CancelRefresh() override;

    private:
        GradientPreviewWidget::SampleFilterFunc m_sampleFilterFunc;
        GradientSampler m_sampler;
        GradientPreviewWidget* m_preview = nullptr;
        GradientPreviewWidget* m_previewWindow = nullptr;

        AZ::EntityId m_observerEntityStub;
        LmbrCentral::DependencyMonitor m_dependencyMonitor;
        bool m_refreshInProgress = false;
    };

    class GradientPreviewDataWidgetHandler
        : public AzToolsFramework::GenericPropertyHandler<GradientPreviewDataWidget>
    {
    public:
        AZ_CLASS_ALLOCATOR(GradientPreviewDataWidgetHandler, AZ::SystemAllocator, 0);

        AZ::u32 GetHandlerName() const override;
        bool ReadValueIntoGUI(size_t index, GradientPreviewDataWidget* GUI, void* value, const AZ::Uuid& propertyType) override;
        void ConsumeAttribute(GradientPreviewDataWidget* GUI, AZ::u32 attrib, AzToolsFramework::PropertyAttributeReader* attrValue, const char* debugName);
        QWidget* CreateGUI(QWidget* pParent) override;

        static void Register();
        static void Unregister();

    private:
        static GradientPreviewDataWidgetHandler* s_instance;
    };
}
