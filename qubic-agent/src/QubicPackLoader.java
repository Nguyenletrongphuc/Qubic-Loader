import net.minecraft.server.packs.repository.*;
import net.minecraft.server.packs.*;
import net.minecraft.server.packs.PackLocationInfo;
import net.minecraft.server.packs.PackSelectionConfig;
import net.minecraft.network.chat.Component;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.lang.reflect.Field;
import java.util.*;

public class QubicPackLoader {
    public static void InjectQubicPack(PackRepository repository) {
        System.out.println("[Qubic] Pack injection called");
        
        try {
            /* get the resources */
            String packPathStr = QubicNative.GetResourcePackPath();
            if (packPathStr == null || packPathStr.isEmpty()) {
                System.err.println("[Qubic] No resource pack path provided!");
                return;
            }
            
            Path packPath = Paths.get(packPathStr);
            System.out.println("[Qubic] Loading Qubic pack from: " + packPath);
            
            /* create a pack info */
            PackLocationInfo locationInfo = new PackLocationInfo(
                "qubic_mods",                           /* id */
                Component.literal("Qubic Loader Mods"), /* title */
                PackSource.BUILT_IN,                    /* source */
                java.util.Optional.empty()              /* knownPackInfo */
            );
            
            /* create ResourcesSupplier using PathResourcesSupplier */
            PathPackResources.PathResourcesSupplier resourcesSupplier = 
                new PathPackResources.PathResourcesSupplier(packPath);
            
            /* ceate PackSelectionConfig - required = true, fixedPosition = true, defaultPosition = TOP */
            PackSelectionConfig selectionConfig = new PackSelectionConfig(
                true,              // required (always enabled)
                Pack.Position.TOP, // defaultPosition (highest priority)
                true               // fixedPosition (can't be moved)
            );
            
            /* read metadata and create pack */
            Pack qubicPack = Pack.readMetaAndCreate(
                locationInfo,
                resourcesSupplier,
                PackType.CLIENT_RESOURCES,
                selectionConfig
            );
            
            if (qubicPack == null) {
                System.err.println("[Qubic] Failed to create pack - check pack.mcmeta!");
                return;
            }
            
            /* use reflection to inject into the 'available' map */
            try {
                Field availableField = PackRepository.class.getDeclaredField("available");
                availableField.setAccessible(true);
                
                @SuppressWarnings("unchecked")
                Map<String, Pack> available = (Map<String, Pack>) availableField.get(repository);
                
                /* make a copy and add pack */
                Map<String, Pack> newAvailable = new HashMap<>(available);
                newAvailable.put("qubic_mods", qubicPack);
                
                /* set as immutable map */
                availableField.set(repository, com.google.common.collect.ImmutableMap.copyOf(newAvailable));
                
                System.out.println("[Qubic] Pack added to available packs");
            } catch (Exception e) {
                System.err.println("[Qubic] Failed to add pack to available: " + e.getMessage());
                e.printStackTrace();
                return;
            }
            
            /* the pack is built in so its kept loaded forever and you cannot disable it! */
            try {
                Field selectedField = PackRepository.class.getDeclaredField("selected");
                selectedField.setAccessible(true);
                
                @SuppressWarnings("unchecked")
                List<Pack> selected = (List<Pack>) selectedField.get(repository);
                List<Pack> newSelected = new ArrayList<>(selected);
                
                /* put at highest piority */
                if (!newSelected.contains(qubicPack)) {
                    newSelected.add(0, qubicPack);
                }
                selectedField.set(repository, com.google.common.collect.ImmutableList.copyOf(newSelected));
                
                System.out.println("[Qubic] Pack added to selected packs with TOP priority");
            } catch (Exception e) {
                System.err.println("[Qubic] Failed to add pack to selected: " + e.getMessage());
                e.printStackTrace();
            }
            
            System.out.println("[Qubic] Successfully injected Qubic resource pack!");
            
        } catch (Exception e) {
            System.err.println("[Qubic] Pack injection failed: " + e.getMessage());
            e.printStackTrace();
        }
    }
}