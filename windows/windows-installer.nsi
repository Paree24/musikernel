; Script generated by the HM NIS Edit Script Wizard.
 
; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "MusiKernel"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "MusiKernel Team"

RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
 
SetCompressor /SOLID lzma
 
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "musikernel-installer-win-x64.exe"
InstallDir "C:\musikernel1-64"
ShowInstDetails show
 
Section "install"
  RMDir /r $INSTDIR
  SetOutPath $INSTDIR
  writeUninstaller "$INSTDIR\uninstall.exe"
  File /r "C:\musikernel1-64\*" 
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  createShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" \
    "$INSTDIR\mingw64\bin\python3.exe $INSTDIR\mingw64\bin\musikernel1" "" "$INSTDIR\musikernel1.ico"
SectionEnd

Section "uninstall"
  RMDir /r $INSTDIR
  delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"
SectionEnd

