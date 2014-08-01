#defineTest(qtcAddDeployment) {
#for(deploymentfolder, DEPLOYMENTFOLDERS) {
#    item = item$${deploymentfolder}
#    greaterThan(QT_MAJOR_VERSION, 4) {
#        itemsources = $${item}.files
#    }
#    $$itemsources = $$eval($${deploymentfolder}.source)
#    itempath = $${item}.path
#    $$itempath= $$eval($${deploymentfolder}.target)
#    export($$itemsources)
#    export($$itempath)
#    DEPLOYMENT += $$item
#    message(Including $$itempath)
#}
#}

MAINPROFILEPWD = $$PWD

#android-no-sdk {
#    for(deploymentfolder, DEPLOYMENTFOLDERS) {
#        item = item$${deploymentfolder}
#        itemfiles = $${item}.files
#        $$itemfiles = $$eval($${deploymentfolder}.source)
#        itempath = $${item}.path
#        $$itempath = /data/user/qt/$$eval($${deploymentfolder}.target)
#        export($$itemfiles)
#        export($$itempath)
#        INSTALLS += $$item
#    }
#
#    target.path = /data/user/qt
#
#    export(target.path)
#    INSTALLS += target
#} else:android {
#    for(deploymentfolder, DEPLOYMENTFOLDERS) {
#        item = item$${deploymentfolder}
#        itemfiles = $${item}.files
#        $$itemfiles = $$eval($${deploymentfolder}.source)
#        itempath = $${item}.path
#        $$itempath = /assets/$$eval($${deploymentfolder}.target)
#        export($$itemfiles)
#        export($$itempath)
#        INSTALLS += $$item
#    }
#
#    x86 {
#        target.path = /libs/x86
#    } else: armeabi-v7a {
#        target.path = /libs/armeabi-v7a
#    } else {
#        target.path = /libs/armeabi
#    }
#
#    export(target.path)
#    INSTALLS += target
#} else:
win32 {
    copyCommand =
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        source = $$MAINPROFILEPWD/$$eval($${deploymentfolder}.source)
        source = $$replace(source, /, \\)
        sourcePathSegments = $$split(source, \\)
        target = $$OUT_PWD/$$eval($${deploymentfolder}.target)/$$last(sourcePathSegments)
        target = $$replace(target, /, \\)
        target ~= s,\\\\\\.?\\\\,\\,
        !isEqual(source,$$target) {
            !isEmpty(copyCommand):copyCommand += &&
            isEqual(QMAKE_DIR_SEP, \\) {
                copyCommand += $(COPY_DIR) \"$$source\" \"$$target\"
            } else {
                source = $$replace(source, \\\\, /)
                target = $$OUT_PWD/$$eval($${deploymentfolder}.target)
                target = $$replace(target, \\\\, /)
                copyCommand += test -d \"$$target\" || mkdir -p \"$$target\" && cp -r \"$$source\" \"$$target\"
            }
        }
    }
    !isEmpty(copyCommand) {
        copyCommand = @echo Copying application data... && @echo $$copyCommand && $$copyCommand
        copydeploymentfolders.commands = $$copyCommand
        first.depends = $(first) copydeploymentfolders
        export(first.depends)
        export(copydeploymentfolders.commands)
        QMAKE_EXTRA_TARGETS += first copydeploymentfolders
    }
}

android-no-sdk {
    target.path = /data/user/qt
    export(target.path)
    INSTALLS += target
} else:android {
    x86 {
        target.path = /libs/x86
    } else: armeabi-v7a {
        target.path = /libs/armeabi-v7a
    } else {
        target.path = /libs/armeabi
    }
    export(target.path)
    INSTALLS += target
} else:unix {
    isEmpty(target.path) {
        qnx {
            target.path = /tmp/$${TARGET}/bin
        } else {
            target.path = /opt/$${TARGET}/bin
        }
        export(target.path)
    }
    INSTALLS += target
}

#defineTest(copyToDestdir) {
#    files = $$1
#
#    for(FILE, files) {
#        DDIR = $$OUT_PWD#
#
#        # Replace slashes in paths with backslashes for Windows
#        win32:FILE ~= s,/,\\,g
#        win32:DDIR ~= s,/,\\,g#
#
#        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
#    }
#
#    export(QMAKE_POST_LINK)
#}

export (INSTALLS)
export (QMAKE_EXTRA_TARGETS)
