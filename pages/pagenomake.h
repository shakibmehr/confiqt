#ifndef PAGENOMAKE_H
#define PAGENOMAKE_H

#include "ui_pagenomake.h"
#include "wizardpagebase.h"

class PageNoMake : public WizardPageBase, private Ui::PageNoMake
{
    Q_OBJECT

public:
    explicit PageNoMake(ConfigManager *config, QWidget *parent = nullptr);
private slots:
    void config_configuresUpdated();
};

#endif // PAGENOMAKE_H