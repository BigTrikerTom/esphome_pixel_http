import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_LIGHT_ID
from esphome.components import light

pixel_http_ns = cg.esphome_ns.namespace("pixel_http")
PixelHttp = pixel_http_ns.class_("PixelHttp", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(PixelHttp),
    cv.Required(CONF_LIGHT_ID): cv.use_id(light.AddressableLight),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    light_var = await cg.get_variable(config[CONF_LIGHT_ID])
    cg.add(var.set_light(light_var))
