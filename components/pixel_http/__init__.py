import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CONF_NUM_LEDS = "num_leds"
CONF_PIN = "pin"
CONF_PORT = "port"

pixel_http_ns = cg.esphome_ns.namespace("pixel_http")
PixelHTTPComponent = pixel_http_ns.class_("PixelHTTPComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(PixelHTTPComponent),
    cv.Required(CONF_NUM_LEDS): cv.int_range(min=1, max=4096),
    cv.Required(CONF_PIN): cv.int_range(min=0, max=39),
    cv.Optional(CONF_PORT, default=80): cv.port,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_NUM_LEDS],
        config[CONF_PIN],
        config[CONF_PORT],
    )
    await cg.register_component(var, config)
