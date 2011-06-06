TEMPLATE = subdirs
SUBDIRS          += \
                 src/TestBuilder \
                 src/Examenator \
                 src/Depot \
                 src/plugins/database \
                 src/plugins/puzzlequest \
                 src/plugins/reports

android-g++ {
OTHER_FILES += \
    android/src/eu/licentia/necessitas/industrius/QtApplication.java \
    android/src/eu/licentia/necessitas/industrius/QtSurface.java \
    android/src/eu/licentia/necessitas/industrius/QtActivity.java \
    android/src/eu/licentia/necessitas/ministro/IMinistroCallback.aidl \
    android/src/eu/licentia/necessitas/ministro/IMinistro.aidl \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/AndroidManifest.xml
}
