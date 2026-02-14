#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/clk.h>

#define DRIVER_NAME "bcm2711-pwm"

#define PWM_CTL   0x00
#define PWM_RNG1  0x10
#define PWM_DAT1  0x14

// global static table
static const struct of_device_id bcm_dt_ids[] = {
    { .compatible = "robert,bcm2711-pwm", },
    { }
};
MODULE_DEVICE_TABLE(of, bcm_dt_ids);

static int bcm2711_pwm_probe(struct platform_device *pdev);
static int bcm2711_pwm_remove(struct platform_device *pdev);
static int bcm2711_pwm_apply(struct pwm_chip *chip,
                             struct pwm_device *pwm,
                             const struct pwm_state *state); 

                

static struct platform_driver bcm2711_pwm_driver = {
    .driver = {
        .name = "bcm2711-pwm",
        .of_match_table = bcm_dt_ids,
    },
    .probe = bcm2711_pwm_probe,
    .remove = bcm2711_pwm_remove,
};


struct bcm2711_pwm {
    struct pwm_chip chip;
    void __iomem *base;
    struct clk *clk;
};

module_platform_driver(bcm2711_pwm_driver);

MODULE_LICENSE("GPL");

static inline struct bcm2711_pwm * to_bcm(struct pwm_chip *chip) {
    return container_of(chip, struct bcm2711_pwm, chip);
}


static int bcm2711_pwm_apply(struct pwm_chip *chip,
                             struct pwm_device *pwm,
                             const struct pwm_state *state) {

    pr_info("bcm2711_pwm_apply called \n");  
    struct bcm2711_pwm *priv = to_bcm(chip);
    u32 period_cycles;
    u32 duty_cycles;
    u32 clk_rate;

    if (!state->enabled) {
        writel(0, priv->base + PWM_CTL);
        return 0;
    }

    clk_rate = clk_get_rate(priv->clk);

    period_cycles = div_u64((u64)clk_rate * state->period, 1000000000ULL);
    duty_cycles   = div_u64((u64)clk_rate * state->duty_cycle, 1000000000ULL);

    writel(period_cycles, priv->base + PWM_RNG1);
    writel(duty_cycles,   priv->base + PWM_DAT1);

    writel(0x81, priv->base + PWM_CTL);  // enable + PWM mode

    return 0;
}


static int bcm2711_pwm_probe(struct platform_device *pdev) {
    pr_info("bcm2711_pwm: probe() called for device %s\n", dev_name(&pdev->dev));
    struct bcm2711_pwm *priv;
    struct resource *res;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    priv->base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(priv->base))
        return PTR_ERR(priv->base);

    priv->clk = devm_clk_get(&pdev->dev, NULL);
    if (IS_ERR(priv->clk))
        return PTR_ERR(priv->clk);

    clk_prepare_enable(priv->clk);

    priv->chip.dev = &pdev->dev;
    priv->chip.ops = &(struct pwm_ops) {
        .apply = bcm2711_pwm_apply,
        .owner = THIS_MODULE,
    };
    priv->chip.npwm = 1;

    return devm_pwmchip_add(&pdev->dev, &priv->chip);
}

static int bcm2711_pwm_remove(struct platform_device *pdev) {
    pr_info("bcm2711_pwm: remove() called\n");
    return 0; 
}