"""UNRUN source-bound native extension bridge; C++ extension remains unfinished.

The native module must be built from this reviewed source and admitted as an
actual compiler/loader input. Importing a same-named module is not qualification.
There is no public JSON/argv/ENV fallback, compatibility adapter or fake backend.
"""
import importlib

class NativeSession:
    __slots__ = ('_module', '_original', '_failed_original', '_attempted')
    def __init__(self, original_native_session):
        # This import is itself an independently admitted original interpreter /
        # extension / loader route. This source has not been imported or run.
        module = importlib.import_module('template_creator_native')
        # Native source type check must inspect its own non-forgeable extension
        # object, not a caller-supplied Boolean or Python class attribute.
        module.require_original_session(original_native_session)
        self._module = module
        self._original = original_native_session
        self._failed_original = None
        self._attempted = False

    def _call(self, operation, *originals):
        # The original native object owns raw results before a Python allocation
        # or conversion can fail. Keep the first original Python bridge exception
        # separately; native output/error and disposal originals remain native.
        try:
            return self._module.invoke_original(self._original, operation, originals)
        except BaseException as original:
            if self._failed_original is None:
                self._failed_original = original
            raise

    def adopt_original_ingress(self, ingress, phase, roles):
        if self._attempted:
            raise ValueError('original native session adoption already attempted')
        self._attempted = True
        return self._call('adopt-original-ingress', ingress, phase.value,
                          tuple(role.value for role in roles))

    def precharge_complete_route(self, steps):
        return self._call('precharge-complete-route', steps)

    def capture_input_classes(self, ordinal, step):
        return self._call('capture-input-classes', ordinal, step)

    def bind_initial_inputs(self, inputs, snapshots):
        return self._call('bind-initial-inputs', inputs, tuple(snapshots))

    def prepare_original_sink(self):
        return self._call('prepare-original-sink')

    def preowned_command(self, ordinal):
        return self._call('preowned-command', ordinal)

    def begin_original(self, owner, snapshot):
        return self._call('begin-original', owner, snapshot)

    def step_original(self, owner):
        return self._call('step-original', owner)

    def capture_to_original_sink(self, owner):
        return self._call('capture-to-original-sink', owner)

    def original_command_disposed(self, owner):
        # Qualification remains distinct from disposal. Native method must
        # derive this from its retained owner, never the passed object fields.
        return self._call('original-command-disposed', owner)

    def verify_original_command_result(self, owner, step):
        return self._call('verify-original-command-result', owner, step)

    def dispose_independently_eligible(self, owner):
        return self._call('dispose-independently-eligible', owner)

    def verify_input_disposal_eligible(self, ordinal, held):
        return self._call('verify-input-disposal-eligible', ordinal, held)

    def capture_creator_originals(self, originals, failures):
        return self._call('capture-creator-originals', originals, failures)

    def flush_readback_original_sink(self):
        return self._call('flush-readback-original-sink')

    def verify_complete_native_closure(self):
        return self._call('verify-complete-native-closure')

    def private_original(self):
        return self._original, self._failed_original

# The corresponding source-owned native extension/type/invoke dispatch is still
# being implemented. This bridge cannot make an absent extension usable or turn
# an extension return into original caller/root/tool/loaded-memory authority.
