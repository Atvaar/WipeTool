#include "wiperclass.h"

//default Constructor === set base object to work with declarations and connections
wiperClass::wiperClass(QObject *parent,validData *thisDrive) : QObject(parent)
{
    //qDebug("I live!!");
    autoStart = false;
    file = new std::fstream();//for making the log
    toolBox = new QProcess(this);//process for system calls in test
    critical = new validData[critVals];//array to pass the critical values for the final test status
    emit statusUpdate(0, "Empty", 4, false);
}

//Destructor === destroy pointers to free up memory
wiperClass::~wiperClass(){
    //qDebug("I dyin'");
    delete file;
    toolBox->kill();
    toolBox->close();
    delete toolBox;
    delete critical;
}

//Public Slots
void wiperClass::killProcess(){
    toolBox->kill();
}

void wiperClass::recieveDrive(QString drive){
    whichDrive = drive;
    //qDebug("emit recieved");
    idDrive();
    if (autoStart){
        startDrive();
    }

}

void wiperClass::startDrive(){
    //open log file
    //later replace with date and drive serial
    file->open("./disk.log",ios::app);

    bool testFlag;

    testFlag = smartDrive1();
    if (testFlag){
        emit statusUpdate(0, thisDrive.driveSerial + ":starting smart", 1, false);
        //if fail then show fail
        qDebug("starting SMART-1");
        //QProcess smartctl whichDrive
        //
    } else {emit statusUpdate(0, thisDrive.driveSerial + ": FAILED", 3, true);}

//    testFlag = initWipe();
//    if (!testFlag){
//        //if fail then show fail
//    }

//    testFlag = smartDrive2();
//    if (!testFlag){
//        //if fail then show fail
//    }
//    bool final = testStatus();

//    *file << "\nTest Status:" <<final;
//    emit testFinished(final);
    file->close();
}

//Private Functions
void wiperClass::idDrive(){
    int snipStart;
    int snipEnd;
    QString getMe = QString("sudo hdparm -I ") + whichDrive;
    //qDebug() << getMe;
    QProcess findSlot;
    findSlot.start(getMe);
    findSlot.waitForFinished(10000);
    getMe = findSlot.readAllStandardOutput();
    snipStart = getMe.indexOf("Serial Number:      ");
    snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
    thisDrive.driveSerial = getMe.mid(snipStart, snipEnd);
    //qDebug() << "Serial Number:      "<< thisDrive.driveSerial;

    //now get drive model
    snipStart = getMe.indexOf("Model Number:       ");
    snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
    thisDrive.driveModel = getMe.mid(snipStart, snipEnd);
    //qDebug() << "Model Number:       " << thisDrive.driveModel;

    //now get drive size
    snipStart = getMe.indexOf("device size with M = 1000*1000:");
    snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
    thisDrive.driveSize = getMe.mid(snipStart, snipEnd);
    //qDebug() << "Drive Size:         " << thisDrive.driveSize;

    //now FW
    snipStart = getMe.indexOf("Firmware Revision:  ");
    snipEnd = getMe.indexOf("\n",snipStart,Qt::CaseInsensitive) - snipStart;
    thisDrive.driveFW = getMe.mid(snipStart, snipEnd);
    //qDebug() << "Firmware Revision:  " << thisDrive.driveFW;

    emit statusUpdate(0, thisDrive.driveSerial, 0, true);//####change this
//    bool status = true;//add if clause to see if did or did not get info and true if good false if failed

//    return status;
}

bool wiperClass::smartDrive1(){
    qDebug("running smart check");
    toolBox->start("sudo smartctl -a /dev/sdb");
    qDebug("waiting for smart");
    toolBox->waitForFinished(3000);
    qDebug("smart check complete");
    QString smartData = toolBox->readAllStandardOutput();
    qDebug() << smartData;
    *file << smartData.toStdString().c_str();//log

    //change to new emit
    //emit statusUpdate(smartData);
    bool status = true;
    return status;
}

void wiperClass::removeDrive(QString drive){
    qDebug("made it this far");
    killProcess();
    emit statusUpdate(0, "Empty", 4, false);

//    if (whichDrive == drive){
//        whichDrive = "";
//        emit statusUpdate(0, "Empty", 4, false);
//    }
//    else {
//        //fix to verify
//        whichDrive = "error";
//        emit error("failed for removing wrong drive");
//    }
}


bool wiperClass::rmDCO(){//hdparm "remove DCO if present"
    //qDebug("running smart check");
    //toolBox->start("sudo smartctl -a /dev/sdb");
    //qDebug("waiting for smart");
    //toolBox->waitForFinished(3000);
    //qDebug("smart check complete");
    //QString smartData = toolBox->readAllStandardOutput();
    //qDebug() << smartData;
    //*file << smartData.toStdString().c_str();//log
    //emit statusUpdate(smartData);
    bool status = true;
    return status;
}

bool wiperClass::rmHPA(){//hdparm "remove HPA if present"
    //qDebug("running smart check");
    //toolBox->start("sudo smartctl -a /dev/sdb");
    //qDebug("waiting for smart");
    //toolBox->waitForFinished(3000);
    //qDebug("smart check complete");
    //QString smartData = toolBox->readAllStandardOutput();
    //qDebug() << smartData;
    //*file << smartData.toStdString().c_str();//log
    //emit statusUpdate(smartData);
    bool status = true;
    return status;
}

bool wiperClass::initWipe(){
    //set security for the drive
    qDebug("initializing Security Features");
    toolBox->start("sudo hdparm --user-master u --security-set-pass Eins /dev/sdb");
    qDebug("waiting for security to set password");
    toolBox->waitForFinished(-1);
    qDebug("security set");
    QString smartData = toolBox->readAllStandardOutput();
    qDebug() << smartData;

    //change to new emit
    //emit statusUpdate(smartData);

    //fire off the wipe event
    qDebug("initiating Secure Erase");

    //change to new emit
    //emit statusUpdate("Secure Erase is erasing the data.  Will continue when complete.");
    toolBox->start("sudo time hdparm --user-master u --yes-i-know-what-i-am-doing --security-erase Eins /dev/sdb");
    qDebug("waiting for Secure Erase to start");
    qDebug("Secure Erase underway");
    toolBox->waitForFinished(-1);
    smartData = toolBox->readAllStandardOutput();
    qDebug() << smartData;

    //change to new emit
    //emit statusUpdate(smartData);

    bool status = true;
    return status;
}

bool initWipe(QString newMethod){//for other wipe methods
    //find newMeshod QString in methods folder and load procedure
    bool status = true;
    return status;
}

bool wiperClass::smartDrive2(){
    qDebug("running 2nd smart check");
    toolBox->start("sudo smartctl -a /dev/sdb");
    qDebug("waiting for smart");
    toolBox->waitForFinished(3000);
    qDebug("2nd smart check complete");
    QString smartData = toolBox->readAllStandardOutput();
    qDebug() << smartData;
    *file << smartData.toStdString().c_str();//log


    //change to new emit
    //emit statusUpdate(smartData);
    bool status = true;
    return status;
}


bool wiperClass::testStatus(){
    bool passFail =false;
    //get formula - use Acronis formula for 9 stats 100%
    //for each stat apply weight
    //for each stat apply formula
    //if pass set passFail true if fail set false
    passFail = true;
    return passFail;
}
