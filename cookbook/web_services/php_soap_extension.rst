.. index::
    single: Web Services; SOAP

Symfony2 コントローラで SOAP ウェブサービスを作成する方法
=========================================================

SOAP サーバとして動作するコントローラをセットアップするには、数個のツールを使うだけで簡単にできます。まず `PHP_SOAP`_ エクステンションをインストールしておく必要があります。この記事の執筆時点の PHP SOAP エクステンションは、 WSDL を生成することができないので、自分で書くか、サードパーティのジェネレータを使用する必要があります。

.. note::

    PHP で SOAP のサーバを実装するには、複数の選択肢があります。 `ZEND SOAP`_ や `NuSOAP` はその例です。この例では、 PHP SOAP エクステンションを使用しますが、基本的な考えはこれらの他の実装にも適用できます。

SOAP は、 PHP オブジェクトのメソッドを外部のエンティティ(SOAP サービスを使用しているユーザ)にさらすことによって、動作します。まず、 SOAP サービスでさらすことになる機能の ``HelloService`` クラスを作成します。このケースでは SOAP サービスはメールを送信する ``hello`` のメソッドを、クライアントが呼ぶことができるようにしています。
::

    namespace Acme\SoapBundle;

    class HelloService
    {
        private $mailer;

        public function __construct(\Swift_Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function hello($name)
        {
            
            $message = \Swift_Message::newInstance()
                                    ->setTo('me@example.com')
                                    ->setSubject('Hello Service')
                                    ->setBody($name . ' says hi!');

            $this->mailer->send($message);


            return 'Hello, ' . $name;
        }

    }

次に、　Symfony にこのクラスのインスタンスを作成できるように修正しましょう。メール送信のクラスなので、 ``Swift_Mailer`` のインスタンスを受け取ることができるように設計します。サービスコンテナを使用すれば、 Symfony に ``HelloService`` オブジェクトを適切に作成する設定をすることができます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml    
        services:
            hello_service:
                class: Acme\DemoBundle\Services\HelloService
                arguments: [mailer]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
         <service id="hello_service" class="Acme\DemoBundle\Services\HelloService">
          <argument>mailer</argument>
         </service>
        </services>

以下は、 SOAP リクエストを扱うことのできるコントローラの例です。 ``indexAction()`` が ``/soap`` ルートを通してアクセスができれば、 WSDL ドキュメントは、 ``/soap?wsdl``` を通して取得することができます。

.. code-block:: php

    namespace Acme\SoapBundle\Controller;
    
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloServiceController extends Controller 
    {
        public function indexAction()
        {
            $server = new \SoapServer('/path/to/hello.wsdl');
            $server->setObject($this->get('hello_service'));
            
            $response = new Response();
            $response->headers->set('Content-Type', 'text/xml; charset=ISO-8859-1');
            
            ob_start();
            $server->handle();
            $response->setContent(ob_get_clean());
            
            return $response;
        }
    }

``ob_start()`` と ``ob_get_clean()`` を呼ぶようにしてください。この２つのメソッドは、 `output.buffering`_ を制御して、 ``$server->handle()`` の出力をまとめることができます。 Symfony はコントローラが "コンテント" としての出力を持つ ``Response`` オブジェクトを返すことを想定しているので、このバッファリングが必要になります。また、クライアントが想定するように "Content-Type" ヘッダを "text/xml" にすることも忘れないでください。 ``ob_start()`` メソッドを使用し、 STDOUT のバッファリングを始め、 ``ob_get_clean()`` メソッドで Response のコンテントに溜まったバッファを全部吐き出し、そして出力バッファをクリアします。そしてその結果、 ``Response`` を返す準備ができます。

以下が、 `NuSOAP`_ クライアントを使用してサービスを呼び出す例です。この例では、上記のコントローラの ``indexAction`` が ``/soap`` ルートでアクセスができることを想定しています。
::
route ``/soap``::

    $client = new \soapclient('http://example.com/app.php/soap?wsdl', true);
    
    $result = $client->call('hello', array('name' => 'Scott'));

WSDL の例は以下の通りです。

.. code-block:: xml

    <?xml version="1.0" encoding="ISO-8859-1"?>
     <definitions xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" 
         xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
         xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/" 
         xmlns:tns="urn:arnleadservicewsdl" 
         xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/" 
         xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/" 
         xmlns="http://schemas.xmlsoap.org/wsdl/" 
         targetNamespace="urn:helloservicewsdl">
      <types>
       <xsd:schema targetNamespace="urn:hellowsdl">
        <xsd:import namespace="http://schemas.xmlsoap.org/soap/encoding/" />
        <xsd:import namespace="http://schemas.xmlsoap.org/wsdl/" />
       </xsd:schema>
      </types>
      <message name="helloRequest">
       <part name="name" type="xsd:string" />
      </message>
      <message name="helloResponse">
       <part name="return" type="xsd:string" />
      </message>
      <portType name="hellowsdlPortType">
       <operation name="hello">
        <documentation>Hello World</documentation>
        <input message="tns:helloRequest"/>
        <output message="tns:helloResponse"/>
       </operation>
      </portType>
      <binding name="hellowsdlBinding" type="tns:hellowsdlPortType">
      <soap:binding style="rpc" transport="http://schemas.xmlsoap.org/soap/http"/>
      <operation name="hello">
       <soap:operation soapAction="urn:arnleadservicewsdl#hello" style="rpc"/>
       <input>
        <soap:body use="encoded" namespace="urn:hellowsdl" 
            encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
       </input>
       <output>
        <soap:body use="encoded" namespace="urn:hellowsdl" 
            encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
       </output>
      </operation>
     </binding>
     <service name="hellowsdl">
      <port name="hellowsdlPort" binding="tns:hellowsdlBinding">
       <soap:address location="http://example.com/app.php/soap" />
      </port>
     </service>
    </definitions>


.. _`PHP SOAP`:          http://php.net/manual/en/book.soap.php
.. _`NuSOAP`:            http://sourceforge.net/projects/nusoap
.. _`output buffering`:  http://php.net/manual/en/book.outcontrol.php
.. _`Zend SOAP`:         http://framework.zend.com/manual/en/zend.soap.server.html

.. 2011/11/08 ganchiku 368dcd210e12ce685bbc91ae2cd409c13c61e6c7

