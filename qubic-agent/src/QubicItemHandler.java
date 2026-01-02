import net.minecraft.world.InteractionResult;
import net.minecraft.world.InteractionHand;
import net.minecraft.world.entity.player.Player;
import net.minecraft.world.item.ItemStack;
import net.minecraft.world.level.Level;

public class QubicItemHandler {
    /* used in c */
    public static native boolean OnItemUse(String itemId, Object player, Object world, Object stack);
    
    /* called from custom Item.use() override */
    public static InteractionResult handleUse(Level world, Player player, InteractionHand hand, String itemId) {
        ItemStack stack = player.getItemInHand(hand);
        
        boolean success = OnItemUse(itemId, player, world, stack);
        
        return success ? InteractionResult.SUCCESS : InteractionResult.PASS;
    }
}