from typing import Union

from . import utilities


class _LocationTaggedException(Exception):
    def __init__(self, msg: str, start_location: Union[utilities.Location, None],
                 end_location: Union[utilities.Location, None]):
        self.msg = msg
        self.start_location = start_location
        self.end_location = end_location

    def __repr__(self):
        return str(self)

    def __str__(self):
        if self.start_location is None:
            return '%s' % self.msg
        if self.end_location is None:
            return '%s\nAt: %s' % (self.msg, str(self.start_location))
        else:
            return '%s\nAt: %s -- %s' % (self.msg, str(self.start_location), str(self.end_location))


class ParseException(_LocationTaggedException):
    def __init__(self, msg: str, start_location: Union[utilities.Location, None],
                 end_location: Union[utilities.Location, None]):
        super().__init__("Error while parsing:\n%s" % msg, start_location, end_location)


class ResolutionException(_LocationTaggedException):
    def __init__(self, msg: str, start_location: Union[utilities.Location, None],
                 end_location: Union[utilities.Location, None]):
        super().__init__("Error resolving name:\n%s" % msg, start_location, end_location)


class UnsupportedFeatureException(_LocationTaggedException):
    def __init__(self, msg: str, start_location: Union[utilities.Location, None],
                 end_location: Union[utilities.Location, None]):
        super().__init__("This feature isn't supported yet:\n%s" % msg, start_location, end_location)
