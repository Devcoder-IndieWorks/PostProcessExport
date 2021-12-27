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

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/PPExportMenu.png)

* 레벨에 배치된 PostProcessVolume Actor를 선택해서 **Target PostProcessVolume**에 설정해 주고, **Apply PostProcessVoluem 버튼**으로 적용한다.(**(1)번 참고**)
* 외부에 저장할 JSON 데이터 파일 이름을 **Json File Name**에 설정한다.(**(2)번 참고**)
* JSON 데이터로 외부에 저장할 PostProcessVolume Actor의 속성을 **Field Name** Combobox에서 지정한다.(**(3)번 참고**)

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/FieldName_Combox_UI.png)

* **Export 버튼**으로 위에서 설정한 내용들을 JSON 파일로 저장한다.
* **Impor 버튼**으로 외부에 저장된 JSON 데이터를 불러와 현재 레벨에서 선택한 PostProcessVolume  Actor에 속성을 다시 설정 하는 방법은 **Target PostProcessVolume**에 PostProcessVolume Actor를 설정해서 적용하고, **Json File Name**에 블러올 JSON 파일 이름을 설정해서 Import 버튼으로 적용한다.
* Runtime에 외부에 저장된 JSON 파일에서 PostProcessVolume Actor의 속성을 재설정 하는 방법은 Blueprint API를 사용한다.

![](https://github.com/Devcoder-IndieWorks/PostProcessExport/blob/master/Images/ImportPostProcessSettings_BP_API.png)



## 구현내용



