#include "wiperclass.h"

//default Constructor === set base object to work with declarations and connections
wiperClass::wiperClass(QObject *parent,validData *thisDrive) : QObject(parent)
{
    //qDebug("I live!!");
    file = new std::fstream();//for making the log
    toolBox = new QProcess(this);//process for system calls in test
    critical = new validData[critVals];//array to pass the critical values for the final test status
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

void wiperClass::startDrive(){
    //open log file
    //later replace with date and drive serial
    file->open("./disk.log",ios::app);

    bool testFlag = idDrive();
    if (!testFlag){
        //if fail then show fail
    }

    testFlag = smartDrive1();
    if (!testFlag){
        //if fail then show fail
    }

    testFlag = initWipe();
    if (!testFlag){
        //if fail then show fail
    }

    testFlag = smartDrive2();
    if (!testFlag){
        //if fail then show fail
    }
    bool final = testStatus();
    *file << "\nTest Status:" <<final;
    emit testFinished(final);
    file->close();
}

//Private Functions
bool wiperClass::idDrive(){
    qDebug("running hdparm");
    toolBox->start("sudo hdparm -I /dev/sdb");
    qDebug("waiting for hdparm");
    toolBox->waitForFinished(3000);
    qDebug("hdparm complete");
    QString hdparmData = toolBox->readAllStandardOutput();
    qDebug() << hdparmData;
    *file << hdparmData.toStdString().c_str();//log
    qDebug("saved to file");
    emit statusUpdate(hdparmData);
    bool status = true;
    return status;
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
    emit statusUpdate(smartData);
    bool status = true;
    return status;
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
    emit statusUpdate(smartData);

    //fire off the wipe event
    qDebug("initiating Secure Erase");
    emit statusUpdate("Secure Erase is erasing the data.  Will continue when complete.");
    toolBox->start("sudo time hdparm --user-master u --yes-i-know-what-i-am-doing --security-erase Eins /dev/sdb");
    qDebug("waiting for Secure Erase to start");
    qDebug("Secure Erase underway");
    toolBox->waitForFinished(-1);
    smartData = toolBox->readAllStandardOutput();
    qDebug() << smartData;
    emit statusUpdate(smartData);

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
    emit statusUpdate(smartData);
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
