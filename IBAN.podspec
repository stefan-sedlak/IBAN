Pod::Spec.new do |s|
  s.name             = 'IBAN'
  s.version          = '0.0.1'
  s.summary          = 'NSString IBAN category to provide IBAN validation'

  s.description      = <<-DESC
IBAN category provides:
* mod 97 validation
* country code validation
* length validation according country
* country SEPA membership check
                       DESC

  s.homepage         = 'https://github.com/stefan-sedlak/IBAN'
  s.license          = { :type => 'BSD', :file => 'LICENSE' }
  s.author           = { 'Stefan Sedlak' => 'stefan@sedlak.eu' }
  s.source           = { :git => 'https://github.com/stefan-sedlak/IBAN.git', :tag => s.version.to_s }

  s.ios.deployment_target = '8.0'

  s.source_files = 'IBAN/Classes/**/*'
  s.public_header_files = 'IBAN/Classes/NSString+IBAN.h'
end
