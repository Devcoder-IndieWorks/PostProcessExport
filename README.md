# UE4 PostProcess Volume 속성 Export/Import

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UE4에서 PostProcess Volume Actor에 설정한 속성 중 일부를 선택하여 JSON 데이터로 변환해서 외부에 저장하고, 저장된 JSON 데이터를 불러와 PostProcess Voume  Actor에 적용하는 기능을 제공하는 Plugin.

PostProcess Volume Actor에서 다음과 같은 속성 값만 JSON 데이터로 변환 할 수 있다.

* Built-In Type인 Int32, Bool, Enum, Float 타입 속성
* Struct Type인 FVector4, FLinearColor 타입 속성.

## 사용법

* PPExportPlugin의 Content 폴더에서 PPExportMenu UAsset 파일을 실행 시킨다.

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/Exec_PPVExportMenu.png)

* UE4 Editor에서 PostProcessVolume 익스포트/임포트 메뉴는 UMG를 이용해서 구현 하였다.
* 메뉴를 실행하게 되면 다음그림과 같이 UE4 Editor에 메뉴가 나타난다.

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/PPVExportMenu.png)

* 레벨에 배치된 PostProcessVolume Actor를 선택해서 **Target PostProcessVolume**에 설정해 주고, **Apply PostProcessVoluem 버튼**으로 적용한다.(**(1)번 참고**)
* 외부에 저장할 JSON 데이터 파일 이름을 **Json File Name**에 설정한다.(**(2)번 참고**)
* JSON 데이터로 외부에 저장할 PostProcessVolume Actor의 속성을 **Field Name** Combobox에서 지정한다.(**(3)번 참고**)

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/FieldName_Combox_UI.png)

* **Export 버튼**으로 위에서 설정한 내용들을 JSON 파일로 저장한다.
* **Impor 버튼**으로 외부에 저장된 JSON 데이터를 불러와 현재 레벨에서 선택한 PostProcessVolume  Actor에 속성을 다시 설정 하는 방법은 **Target PostProcessVolume**에 PostProcessVolume Actor를 설정해서 적용하고, **Json File Name**에 블러올 JSON 파일 이름을 설정해서 Import 버튼으로 적용한다.
* Runtime에 외부에 저장된 JSON 파일에서 PostProcessVolume Actor의 속성을 재설정 하는 방법은 Blueprint API를 사용한다.

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/ImportPostProcessSettings_BP_API.png)



## 구현내용

PostProcessVolume Actor에는 많은 속성들이 존재한다. 이 속성들은 저마다 각각 다른 데이터 타입으로 되어져 있으며, 익스포트 할 속성은 사용자가 선택 할 수 있어야 한다.

그래서 Unreal C++라고 불리우는 Unreal Engine 개발자들이 만든(?) Unreal Engine에서만 사용할 수 있는  C++ 확장 기능 중 **Property Reflection** 기능을 이용하여 속성 이름으로 속성값의 데이터 타입을 구하고, 타입에 맞게 JSON 데이터로 변환 할 수 있는 체계를 구현하였다.  또한 **FJsonObject** 라는 JSON 데이터 처리에 대한 기능을 Unreal Engine에서 제공해 주는데, 이 기능을 사용하기 편리하도록 하기 위해 Wrapping하여 **FVJsonFieldData**라는 클래스를 만들었다.

### UnrealC++ Property Reflection

속성의 이름으로 PostProcessVolume Actor의 속성값을 찾기 위해 UnrealC++의 Property Reflection 기능을 이용한다.

우선 UnrealC++에서 Reflection 정보를 가지고 있는 클래스 타입인 UField의 하위 클래스인 UClass, UStruct에서 이름으로 FProperty  데이터를 찾아내야 한다. UE4에서는 **FindFProperty()** 함수가 그러한 기능을 하는데, 이 함수는 클래스 선언시 지정한 멤버변수 이름으로만 FProperty 데이터를 찾을 수 있다. 그런데 PostProcessVolume Actor의 속성이 UE4 Editor에서 표시 될 때는 멤버변수 이름으로 표시되는 것이 아니라 Display Name으로 설정된 것이 표시되게 된다. Display Name로 FindFProperty() 함수를 통해 FProperty 데이터를 찾고자 한다면 찾을 수 없다.

Category와 Display Name이 **콤마(,)기호**로 합쳐진 문자열을 통해 FProperty  데이터를 찾을 수 있는 함수를 구현하였다.

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/Function_FindFPropertyByDisplayName.png)

**콤마(,)기호**로 Category와 Display Name으로 분리된 문자열을 구함(**(1)번 참고**) UStruct or UClass에 있는 Property 중에서 Display Name이 같은 Property가 있는지 확인 후 Category 또한 입력한 것과 같은지 비교해서 해당 FProperty를 리턴한다.(**(2)번 참고**)

이렇게 Display Name으로부터 얻어낸 FProperty 데이터를 이용해서 속성 타입별로 JSON 데이터를 만들기 위한 처리를 하는 함수인 **GetPropertyValueToJsonData()** 함수를 알아본다.

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/Function_GetPropertyValueToJsonData.png)

그림에서 **(1)번**. PostProcessVolume Actor에서 PostProcess 설정 항목이 있는 **Settings**에 대한 Reflection 정보가 있는 **FProperty** 데이터를 얻는 구문이다. 여기에서는 Settings 멤버변수 이름을 사용해서 얻기 때문에 UE4에서 제공해 주는 **FindFProperty()** 함수를 사용한다. 

**(2)번**. (1)번에서 얻은 FProperty 데이터에서 속성에 대한 Reflection 정보가 아닌 **실제 데이터 값**이 저장된 메모리 주소에 대한 정보를 얻는다. 실제 데이터 값이 있는 메모리 주소를 얻기 위해서는 **얻고자 하는 타입의 FProperty 데이터**의 **ContainerPtrToValuePtr()** 함수에 실제 데이터 값을 **포함하고 있는 인스턴스의 메모리 주소**를 전달해서 얻을 수 있다. 

**(3)번**. (1)번에 얻은 FProperty 데이터에서 **Display Name에 해당하는 FProperty 데이터**를 얻는 구문이다. Display Name을 통해 해당 FProperty 데이터를 얻기 위해서는 위에서 설명한 **FindFPropertyByDisplayName()** 함수를 사용한다.

**(4)번**. 이렇게 얻어진 FProperty가 **실제 어떤 타입의 Property**인지 확인하기 위해 Field 타입에 한정된 타입 캐스팅을 한다.

**(5)번**. (2)번과 마찬가지로 **(4)번에서 얻은 FProperty의 실제 데이터 값**이 저장된 메모리 주소에 대한 정보를 얻는다.

**(6)번**. (5)번에 구한 실제 데이터 값이 저장된 메모리 주소에서 **실제 데이터 값을 얻어내는 것**을 보여주고 있다.



Field 타입에 한정된 타입 캐스팅을 통해 Numeric 타입인지, Enum 타입인지, Boolean 타입인지, Struct 타입인지 판별 할 수 있다.

(7)번. Struct 타입 Property 데이터이면 **__GetStructPropertyValue()** 함수를 통해 실제 데이터 값을 얻어온다.

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/Function_GetStructPropertyValue.png)

__GetStructPropertyValue()  함수는 그림에서처럼 전달된 FProperty 데이터의 ContainerPtrToValuePtr() 함수를 통해 데이터 값을 얻는 단순한 구조이다. 



