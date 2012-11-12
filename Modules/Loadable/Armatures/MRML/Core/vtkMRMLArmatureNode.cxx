/*=========================================================================

  Program: Bender

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

// Armatures includes
#include "vtkMRMLArmatureNode.h"
#include "vtkMRMLBoneDisplayNode.h"
#include "vtkMRMLBoneNode.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkArmatureWidget.h>
#include <vtkBoneRepresentation.h>
#include <vtkBoneWidget.h>
#include <vtkCollection.h>
#include <vtkCylinderBoneRepresentation.h>
#include <vtkDoubleConeBoneRepresentation.h>
#include <vtkMathUtilities.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkWidgetRepresentation.h>

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLArmatureNode);

//----------------------------------------------------------------------------
class vtkMRMLArmatureNodeCallback : public vtkCommand
{
public:
  static vtkMRMLArmatureNodeCallback *New()
    { return new vtkMRMLArmatureNodeCallback; }

  vtkMRMLArmatureNodeCallback()
    { this->Node = 0; }

  virtual void Execute(vtkObject* caller, unsigned long eventId, void* data)
    {
    vtkNotUsed(data);
    switch (eventId)
      {
      case vtkCommand::ModifiedEvent:
        {
        this->Node->Modified();
        break;
        }
      }
    }

  vtkMRMLArmatureNode* Node;
};

//----------------------------------------------------------------------------
vtkMRMLArmatureNode::vtkMRMLArmatureNode()
{
  this->ArmatureProperties = vtkArmatureWidget::New();
  this->ArmatureProperties->CreateDefaultRepresentation();
  this->ArmatureProperties->SetBonesRepresentation(
    vtkArmatureWidget::DoubleCone);
  this->WidgetState = vtkArmatureWidget::Rest;
  this->SetHideFromEditors(0);
  this->Callback = vtkMRMLArmatureNodeCallback::New();

  for (int i = 0; i<3; ++i)
    {
    this->Color[i] = 0;
    }
  this->Opacity = 1.0;

  this->Callback->Node = this;
  this->ArmatureProperties->AddObserver(vtkCommand::ModifiedEvent,
    this->Callback);
}

//----------------------------------------------------------------------------
vtkMRMLArmatureNode::~vtkMRMLArmatureNode()
{
  this->ArmatureProperties->Delete();
}

//----------------------------------------------------------------------------
void vtkMRMLArmatureNode::WriteXML(ostream& of, int nIndent)
{
  this->Superclass::WriteXML(of, nIndent);
  // of << indent << " ctrlPtsNumberingScheme=\"" << this->NumberingScheme << "\"";
}

//----------------------------------------------------------------------------
void vtkMRMLArmatureNode::ReadXMLAttributes(const char** atts)
{
  int disabledModify = this->StartModify();

  this->Superclass::ReadXMLAttributes(atts);

  while (*atts != NULL)
    {
    //const char* attName = *(atts++);
    //std::string attValue(*(atts++));

    // if  (!strcmp(attName, "ctrlPtsNumberingScheme"))
    //   {
    //   std::stringstream ss;
    //   ss << attValue;
    //   ss >> this->NumberingScheme;
    //   }
    }
  this->EndModify(disabledModify);
}

//-----------------------------------------------------------
void vtkMRMLArmatureNode::UpdateScene(vtkMRMLScene *scene)
{
  this->Superclass::UpdateScene(scene);
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::ProcessMRMLEvents(vtkObject* caller,
                                        unsigned long event,
                                        void* callData)
{
  this->Superclass::ProcessMRMLEvents(caller, event, callData);
}

//----------------------------------------------------------------------------
void vtkMRMLArmatureNode::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------
void vtkMRMLArmatureNode::Copy(vtkMRMLNode* anode)
{
  int wasModifying = this->StartModify();
  this->Superclass::Copy(anode);
  this->EndModify(wasModifying);
}

//----------------------------------------------------------------------------
const char* vtkMRMLArmatureNode::GetIcon()
{
  return this->Superclass::GetIcon();
}

//----------------------------------------------------------------------------
vtkMRMLBoneNode* vtkMRMLArmatureNode::GetParentBone(vtkMRMLBoneNode* bone)
{
  vtkMRMLDisplayableHierarchyNode* boneHierarchyNode =
    vtkMRMLDisplayableHierarchyNode::GetDisplayableHierarchyNode(
      bone->GetScene(), bone->GetID());
  vtkMRMLDisplayableHierarchyNode* parentHierarchyNode =
    vtkMRMLDisplayableHierarchyNode::SafeDownCast(
      boneHierarchyNode->GetParentNode());
  return vtkMRMLBoneNode::SafeDownCast(
    parentHierarchyNode->GetDisplayableNode());
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::SetBonesRepresentation(int representationType)
{
  this->ArmatureProperties->SetBonesRepresentation(representationType);
}

//---------------------------------------------------------------------------
int vtkMRMLArmatureNode::GetBonesRepresentation()
{
  return this->ArmatureProperties->GetBonesRepresentationType();
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::SetWidgetState(int state)
{
  if (state == this->WidgetState)
    {
    return;
    }

  this->WidgetState = state;
  this->Modified();
}

//---------------------------------------------------------------------------
int vtkMRMLArmatureNode::GetWidgetState()
{
  return this->ArmatureProperties->GetWidgetState();
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::SetShowAxes(int axesVisibility)
{
  this->ArmatureProperties->SetShowAxes(axesVisibility);
}

//---------------------------------------------------------------------------
int vtkMRMLArmatureNode::GetShowAxes()
{
  return this->ArmatureProperties->GetShowAxes();
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::SetShowParenthood(int parenthood)
{
  this->ArmatureProperties->SetShowParenthood(parenthood);
}

//---------------------------------------------------------------------------
int vtkMRMLArmatureNode::GetShowParenthood()
{
  return this->ArmatureProperties->GetShowParenthood();
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::SetVisibility(bool visible)
{
  this->ArmatureProperties->GetRepresentation()->SetVisibility(visible);
}

//---------------------------------------------------------------------------
bool vtkMRMLArmatureNode::GetVisibility()
{
  return this->ArmatureProperties->GetRepresentation()->GetVisibility();
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::SetOpacity(double opacity)
{
  if (vtkMathUtilities::FuzzyCompare(this->Opacity, opacity))
    {
    return;
    }

  this->Opacity = opacity;

  // vv This should go to logic vv
  vtkNew<vtkCollection> bones;
  this->GetAllBones(bones.GetPointer());
  for (int i = 0; i < bones->GetNumberOfItems();++i)
    {
    vtkMRMLBoneNode* boneNode
      = vtkMRMLBoneNode::SafeDownCast(bones->GetItemAsObject(i));
    if (boneNode)
      {
      boneNode->SetOpacity(this->Opacity);
      }
    }
  // ^^ This should go to logic ^^

  this->Modified();
}

//---------------------------------------------------------------------------
double vtkMRMLArmatureNode::GetOpacity()
{
  return this->Opacity;
}

namespace
{
bool CompareColor(int rgb1[3], int rgb2[3])
{
  int diff[3];
  for (int i=0; i<3; ++i)
    {
    diff[i] = rgb1[i] - rgb2[i];
    }

  return diff[0] == 0 && diff[1] == 0 && diff[2] == 0;
}
}// end namespace

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::SetColor(int rgb[3])
{
  if (CompareColor(this->Color, rgb))
    {
    return;
    }

  for (int i=0; i<3; ++i)
    {
    this->Color[i] = rgb[i];
    }

  // vv This should go to logic vv
  vtkNew<vtkCollection> bones;
  this->GetAllBones(bones.GetPointer());
  for (int i = 0; i < bones->GetNumberOfItems();++i)
    {
    vtkMRMLBoneNode* boneNode =
      vtkMRMLBoneNode::SafeDownCast(bones->GetItemAsObject(i));
    if (boneNode)
      {
      boneNode->SetBoneColor(this->Color);
      }
    }
  // ^^ This should go to logic ^^

  this->Modified();
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode::GetColor(int rgb[3])
{
  for (int i=0; i<3; ++i)
    {
    rgb[i] = this->Color[i];
    }
}
/*
//---------------------------------------------------------------------------
void vtkMRMLArmatureNode
::SetBoneLinkedWithParent(vtkBoneWidget* bone, bool linked)
{
  if (CompareColor(this->Color, rgb))
    {
    return;
    }

  for (int i=0; i<3; ++i)
    {
    this->Color[i] = rgb[i];
    }

  double doubleRGB[3];
  doubleRGB[0] = rgb[0]/255.0;
  doubleRGB[1] = rgb[1]/255.0;
  doubleRGB[2] = rgb[2]/255.0;

  // vv This should go to logic vv
  for (int i = 0; i < bones->GetNumberOfItems();++i)
    {
    vtkBoneWidget* bone
      = vtkBoneWidget::Safedowncast(bones->GetItemAsObject(i));
    if (bone && bone->GetBoneRepresentation())
      {
      bone->GetBoneRepresenation()->GetLineProperty()->SetColor(doubleRGB);

      if (vtkCylinderBoneRepresentation::SafeDownCast(
          bone->GetBoneRepresentation()))
        {
        vtkCylinderBoneRepresentation::SafeDownCast(
          bone->GetBoneRepresentation())->GetCylinderProperty()
            ->SetColor(doubleRGB);
        }
      else if (vtkDoubleConeBoneRepresentation::SafeDownCast(
          bone->GetBoneRepresentation()))
        {
        vtkDoubleConeBoneRepresentation::SafeDownCast(
          bone->GetBoneRepresentation())->GetConesProperty()
            ->SetColor(doubleRGB);
        }
      }
    }
  // ^^ This should go to logic ^^

  this->Modified();
}

//---------------------------------------------------------------------------
bool vtkMRMLArmatureNode::GetBoneLinkedWithParent(vtkBoneWidget* bone)
{
  this->ArmatureProperties->
}*/

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode
::CopyArmatureWidgetProperties(vtkArmatureWidget* armatureWidget)
{
  //int wasModifying = this->StartModify(); // ? Probably not
  this->ArmatureProperties->SetBonesRepresentation(
    armatureWidget->GetBonesRepresentationType());
  this->WidgetState = armatureWidget->GetWidgetState();
  this->ArmatureProperties->SetShowAxes(
    armatureWidget->GetShowAxes());
  this->ArmatureProperties->SetShowParenthood(
    armatureWidget->GetShowParenthood());

  // Armature does not have a "real" representation so far
  // -> So no Color or Opacity properties
}

//---------------------------------------------------------------------------
void vtkMRMLArmatureNode
::PasteArmatureNodeProperties(vtkArmatureWidget* armatureWidget)
{
  //int wasModifying = this->StartModify(); // ? Probably not
  armatureWidget->SetBonesRepresentation(
    this->ArmatureProperties->GetBonesRepresentationType());
  armatureWidget->SetWidgetState(this->WidgetState);
  armatureWidget->SetShowAxes(
    this->ArmatureProperties->GetShowAxes());
  armatureWidget->SetShowParenthood(
    this->ArmatureProperties->GetShowParenthood());

  vtkNew<vtkCollection> bones;
  this->GetAllBones(bones.GetPointer());
  for (int i = 0; i < bones->GetNumberOfItems();++i)
    {
    vtkMRMLBoneNode* boneNode =
      vtkMRMLBoneNode::SafeDownCast(bones->GetItemAsObject(i));
    if (boneNode)
      {
      boneNode->SetBoneColor(this->Color);
      boneNode->SetOpacity(this->Opacity);
      }
    }
}
