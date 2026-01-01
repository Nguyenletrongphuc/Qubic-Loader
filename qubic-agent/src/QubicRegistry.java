import net.minecraft.core.Registry;
import net.minecraft.core.registries.BuiltInRegistries;
import net.minecraft.resources.Identifier;
import net.minecraft.resources.ResourceKey;
import net.minecraft.core.registries.Registries;
import net.minecraft.world.item.Item;
import net.minecraft.network.chat.Component;
import net.minecraft.ChatFormatting;
import com.mojang.serialization.Lifecycle;

public class QubicRegistry {
    /**
     * @param modId The mod namespace (e.g., "testmod")
     * @param itemId The item path (e.g., "test-item")
     * @param maxStackSize Maximum stack size (1-64)
     * @param displayName Display name for the item (e.g., "Test Item")
     * @return true if successful, false otherwise
     */
    public static boolean registerItem(String modId, String itemId, int maxStackSize, String displayName, boolean fireResistant, int durability) {
        try {
            System.out.println("[Qubic] Registering item: " + modId + ":" + itemId);
            
            Identifier identifier = Identifier.fromNamespaceAndPath(modId, itemId);
            ResourceKey<Item> key = ResourceKey.create(Registries.ITEM, identifier);
            Item.Properties properties = new Item.Properties();
            properties.setId(key);
            
            if (maxStackSize != 64) { properties.stacksTo(maxStackSize); } /* stacksize flag */
            if (fireResistant) { properties.fireResistant(); }             /* fire resistant flag */
            if (durability > 0) { properties.durability(durability); }     /* durability flag */

            /* here creates the item */
            Item item = new Item(properties) {
                @Override
                public Component getName(net.minecraft.world.item.ItemStack stack) {
                    return Component.literal(displayName);
                }
                
                @Override
                public void appendHoverText(net.minecraft.world.item.ItemStack stack,
                                           net.minecraft.world.item.Item.TooltipContext context,
                                           net.minecraft.world.item.component.TooltipDisplay tooltipDisplay,
                                           java.util.function.Consumer<Component> consumer,
                                           net.minecraft.world.item.TooltipFlag tooltipFlag) 
                {
                    super.appendHoverText(stack, context, tooltipDisplay, consumer, tooltipFlag);
                    
                    /* uhh basically adds a blue italic tag as a description of the item */
                    String modName = modId.substring(0, 1).toUpperCase() + modId.substring(1);
                    consumer.accept(Component.literal(modName).withStyle(ChatFormatting.BLUE, ChatFormatting.ITALIC));
                }
            };
            
            /* register it using Registry.register */
            Registry.register(BuiltInRegistries.ITEM, identifier, item);
            
            System.out.println("[Qubic] Successfully registered: " + modId + ":" + itemId);
            return true;
            
        } catch (Exception e) {
            System.err.println("[Qubic] ERROR registering item " + modId + ":" + itemId);
            e.printStackTrace();
            return false;
        }
    }
    
    /* register an item with default stack size (64) */
    public static boolean registerItem(String modId, String itemId, String displayName) {
        return registerItem(modId, itemId, 64, displayName, false, 0);
    }
}